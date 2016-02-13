// The MIT License (MIT)
//
// Copyright (c) 2016 Erik Hvatum
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Authors: Erik Hvatum <ice.rikh@gmail.com>

#include "SSImage.h"
#include <exception>
#include <cstring>
#include <FreeImagePlus.h>

volatile std::atomic<std::size_t> SSImage::sm_nextSerial{0};

const QList<QStringList> SSImage::smc_componentNames{
    {},
    {"Gr"},
    {"Gr", "A"},
    {"R", "G", "B"},
    {"R", "G", "B", "A"}
};
const QStringList SSImage::smc_componentsStrs{
    "",
    "Gr",
    "GrA",
    "RGB",
    "RGBA"
};

const std::uint8_t SSImage::smc_componentsCounts[SSImage::Components_last + 1] = {
    0,
    1,
    2,
    3,
    4
};

std::size_t SSImage::generateSerial()
{
    return sm_nextSerial++;
}

static void noop_deleter(void*) {}
using get_default_deleter = std::default_delete<std::uint8_t[]>;

class NoReconcile
{
public:
    explicit NoReconcile(SSImage& image) : m_image(image)
    {
        ++m_image.m_noReconcile;
    }
    ~NoReconcile()
    {
        --m_image.m_noReconcile;
    }
private:
    SSImage& m_image;
};

const std::size_t SSImage::DTypeSizes[] = {
    0, // DTypeNull
    1, // DTypeUInt8
    2, // DTypeUInt12
    2, // DTypeUInt16
    4, // DTypeUInt32
    8, // DTypeUInt64
    2, // DTypeFloat16
    4, // DTypeFloat32
    8, // DTypeFloat64
};

SSImage::SSImage(QObject *parent)
  : QObject(parent),
    m_serial(std::numeric_limits<std::size_t>::max()),
    m_noReconcile(0),
    m_isValid(false),
    m_componentDType(DTypeNull),
    m_components(ComponentsNull),
    m_byteCount(0)
{
}

SSImage::SSImage(DType componentDType, const QSize& size, Components components, QObject* parent)
  : QObject(parent),
    m_serial(std::numeric_limits<std::size_t>::max()),
    m_noReconcile(0),
    m_isValid(false),
    m_componentDType(componentDType),
    m_components(components),
    m_size(size),
    m_byteCount(0)
{
    reconcile();
}

SSImage::SSImage(DType componentDType,
                 const std::uint8_t* rawData,
                 const QSize& size,
                 Components components,
                 QObject* parent)
  : QObject(parent),
    m_serial(std::numeric_limits<std::size_t>::max()),
    m_noReconcile(0),
    m_componentDType(componentDType),
    m_components(components),
    m_size(size),
    m_byteCount(0)
{
    RawData _rawData{const_cast<std::uint8_t*>(rawData), noop_deleter};
    init(_rawData, true);
}

SSImage::SSImage(DType componentDType,
                 std::uint8_t* rawData,
                 const QSize& size,
                 Components components,
                 bool takeOwnership,
                 QObject* parent)
  : QObject(parent),
    m_serial(generateSerial()),
    m_noReconcile(0),
    m_isValid(false),
    m_componentDType(componentDType),
    m_components(components),
    m_size(size)
{
    if(takeOwnership)
    {
        RawData _rawData(rawData, get_default_deleter());
        init(_rawData, false);
    }
    else
    {
        RawData _rawData{rawData, noop_deleter};
        init(_rawData, false);
    }
}

SSImage::SSImage(DType componentDType,
                 const RawData& rawData,
                 const QSize& size,
                 Components components,
                 bool copyData,
                 QObject* parent)
  : QObject(parent),
    m_serial(generateSerial()),
    m_noReconcile(0),
    m_isValid(false),
    m_componentDType(componentDType),
    m_components(components),
    m_size(size),
    m_byteCount(0)
{
    init(const_cast<RawData&>(rawData), copyData);
}

SSImage::SSImage(const SSImage &rhs, bool copyData)
  : QObject(rhs.parent()),
    m_serial(rhs.m_serial),
    m_noReconcile(0),
    m_isValid(false),
    m_componentDType(rhs.m_componentDType),
    m_components(rhs.m_components),
    m_size(rhs.m_size),
    m_byteCount(rhs.m_byteCount)
{
    init(const_cast<RawData&>(rhs.m_rawData), copyData);
}

SSImage::~SSImage()
{
}

void SSImage::init(RawData& rawData, bool copyData)
{
    if(m_components == ComponentsNull)
        throw std::invalid_argument("The value supplied to full constructor for components must not be SSImage::ComponentsNull.");
    if(m_size.width() <= 0 || m_size.height() <= 0)
        throw std::invalid_argument("The value supplied to full constructor for size must have positive width and height.");
    m_byteCount =
        static_cast<std::size_t>(m_size.width()) *
        static_cast<std::size_t>(m_size.height()) *
        componentCount() *
        DTypeSizes[m_componentDType];
    if(copyData)
    {
        m_rawData.reset(new std::uint8_t[m_byteCount], get_default_deleter());
        std::memcpy(m_rawData.get(), rawData.get(), m_byteCount);
    }
    else
    {
        m_rawData = rawData;
    }
    m_isValid = true;
}

SSImage& SSImage::operator = (const SSImage& rhs)
{
    if(this != &rhs)
    {
        {
            NoReconcile NR(*this);
            setComponentDType(rhs.m_componentDType);
            setSize(rhs.m_size);
            setComponents(rhs.m_components);
        }
        if(rhs.m_isValid)
        {
            m_rawData = rhs.m_rawData;
            if(rhs.m_byteCount != m_byteCount)
            {
                m_byteCount = rhs.m_byteCount;
                byteCountChanged(m_byteCount);
            }
            if(!m_isValid)
            {
                m_isValid = true;
                isValidChanged(true);
            }
            if(m_serial != rhs.m_serial)
            {
                m_serial = rhs.m_serial;
                serialChanged(m_serial);
            }
        }
        else
        {
            m_rawData.reset();
            reconcile();
        }
    }
    return *this;
}

void SSImage::clear()
{
    setSize(QSize());
}

bool SSImage::operator == (const SSImage& rhs) const
{
    bool ret{false};
    if(this == &rhs)
    {
        ret = true;
    }
    else if (m_componentDType == rhs.m_componentDType
           && m_isValid == rhs.m_isValid
           && m_size == rhs.m_size
           && m_components == rhs.m_components )
    {
        if(m_isValid)
        {
            if(m_componentDType == DTypeUInt12)
            {
                // Compare only lower 12 bits
                // TODO: validate this block
                bool same{true};
                std::uint16_t const *l{(const std::uint16_t*)m_rawData.get()};
                std::uint16_t const *r{(const std::uint16_t*)rhs.m_rawData.get()};
                const std::uint16_t * const l_e{l + m_byteCount/2};
                for(; l != l_e; ++l, ++r)
                {
                    if((*l & 0x0fff) != (*r & 0x0fff))
                    {
                        same = false;
                        break;
                    }
                }
                ret = same;
            }
            else
            {
                ret = std::memcmp(m_rawData.get(), rhs.m_rawData.get(), m_byteCount) == 0;
            }
        }
        else
        {
            ret = true;
        }
    }
    return ret;
}

SSImage::operator bool () const
{
    return isValid();
}

std::size_t SSImage::serial() const
{
    return m_serial;
}

bool SSImage::isValid() const
{
    return m_isValid;
}

SSImage::DType SSImage::componentDType() const
{
    return m_componentDType;
}

void SSImage::setComponentDType(DType componentDType)
{
    if(componentDType != m_componentDType)
    {
        m_componentDType = componentDType;
        reconcile();
        componentDTypeChanged(m_componentDType);
    }
    else reconcile();
}

const SSImage::RawData& SSImage::rawData() const
{
    return m_rawData;
}

const QSize& SSImage::size() const
{
    return m_size;
}

void SSImage::setSize(const QSize& size)
{
    if(size != m_size)
    {
        m_size = size;
        reconcile();
        sizeChanged(m_size);
    }
    else reconcile();
}

SSImage::Components SSImage::components() const
{
    return m_components;
}

void SSImage::setComponents(Components components)
{
    if(components != m_components)
    {
        m_components = components;
        reconcile();
        componentsChanged(m_components);
    }
    else reconcile();
}

std::size_t SSImage::byteCount() const
{
    return m_byteCount;
}

void SSImage::notifyOfDataChange()
{
    m_serial = generateSerial();
    serialChanged(m_serial);
}

bool SSImage::read(const QUrl& furl)
{
    bool ret{false};
    if(!furl.isLocalFile())
    {
        qDebug("Image::read(..): Only file:// URLs are supported at the moment.");
    }
    else
    {
        fipImage fiImage;
        QByteArray fpath{furl.toLocalFile().toUtf8()};
        // TODO: direct support for Ga and RGB images
        // TODO: direct support for non-uint16 images
        if(fiImage.load(fpath.data()))
        {
            if(fiImage.convertToRGBA16())
            {
                std::size_t oldByteCount{m_byteCount};
                {
                    NoReconcile NR(*this);
                    setComponentDType(DTypeUInt16);
                    setSize(QSize(static_cast<int>(fiImage.getWidth()), static_cast<int>(fiImage.getHeight())));
                    setComponents(ComponentsRGBA);
                    m_byteCount = componentCount() * fiImage.getWidth() * fiImage.getHeight() * DTypeSizes[m_componentDType];
                    m_rawData.reset(new std::uint8_t[m_byteCount]);
                    std::memcpy(m_rawData.get(), fiImage.accessPixels(), m_byteCount);
                }
                if(m_byteCount != oldByteCount) byteCountChanged(m_byteCount);
                if(!m_isValid)
                {
                    m_isValid = true;
                    isValidChanged(m_isValid);
                }
                notifyOfDataChange();
                ret = true;
            }
            else
            {
                qDebug("Image::read(..): Failed to convert image to uint16 RGBA.");
            }
        }
    }
    return ret;
}

bool SSImage::write(const QUrl& furl) const
{
    bool ret{false};
    if(!furl.isLocalFile())
    {
        qDebug("Image::write(..): Only file:// URLs are supported at the moment.");
    }
    else
    {
        qDebug("Image::write(..): Writing is not supported at the moment.");
    }
    return ret;
}

void SSImage::reconcile()
{
    if(m_noReconcile == 0)
    {
        const bool valid {
            m_componentDType != DTypeNull &&
            m_size.width() >= 1 && m_size.height() >= 1 &&
            m_components != ComponentsNull
        };
        if(valid != m_isValid)
        {
            if(valid)
            {
                std::size_t byteCount =
                    static_cast<std::size_t>(m_size.width()) *
                    static_cast<std::size_t>(m_size.height()) *
                    componentCount() *
                    DTypeSizes[m_componentDType];
                if(byteCount != m_byteCount)
                {
                    m_byteCount = byteCount;
                    byteCountChanged(m_byteCount);
                    m_rawData.reset(new std::uint8_t[m_byteCount], get_default_deleter());
                    notifyOfDataChange();
                }
            }
            else
            {
                if(m_byteCount != 0)
                {
                    m_byteCount = 0;
                    byteCountChanged(m_byteCount);
                }
                if(m_rawData)
                {
                    m_rawData.reset();
                    notifyOfDataChange();
                }
            }
            isValidChanged(m_isValid);
        }
    }
}
