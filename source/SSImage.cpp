#include "common.h"
#include <exception>
#include <cstring>

#include "SSImage.h"

volatile std::atomic<std::size_t> SSImage::sm_nextSerial{0};

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

const std::size_t SSImage::ImageTypeSizes[] = {
    0, // NullDT
    1, // UInt8DT
    2, // UInt12DT
    2, // UInt16DT
    4, // UInt32DT
    8, // UInt64DT
    4, // Float32DT
    8, // Float64DT
};

SSImage::SSImage(QObject *parent)
  : QObject(parent),
    m_serial(std::numeric_limits<std::size_t>::max()),
    m_noReconcile(0),
    m_isValid(false),
    m_componentType(NullDT),
    m_componentCount(0),
    m_byteCount(0)
{
}

SSImage::SSImage(DType imageType, const QSize& size, std::size_t componentCount, QObject* parent)
  : QObject(parent),
    m_serial(std::numeric_limits<std::size_t>::max()),
    m_noReconcile(0),
    m_isValid(false),
    m_componentType(imageType),
    m_size(size),
    m_componentCount(componentCount),
    m_byteCount(0)
{
    reconcile();
}

SSImage::SSImage(DType imageType,
                 const std::uint8_t* rawData,
                 const QSize& size,
                 std::size_t componentCount,
                 QObject* parent)
  : QObject(parent),
    m_serial(std::numeric_limits<std::size_t>::max()),
    m_noReconcile(0),
    m_componentType(imageType),
    m_size(size),
    m_componentCount(componentCount),
    m_byteCount(0)
{
    RawData _rawData{const_cast<std::uint8_t*>(rawData), noop_deleter};
    init(_rawData, true);
}

SSImage::SSImage(DType imageType,
                 std::uint8_t* rawData,
                 const QSize& size,
                 std::size_t componentCount,
                 bool takeOwnership,
                 QObject* parent)
  : QObject(parent),
    m_serial(generateSerial()),
    m_noReconcile(0),
    m_isValid(false),
    m_componentType(imageType),
    m_size(size),
    m_componentCount(componentCount)
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

SSImage::SSImage(DType imageType,
                 const RawData& rawData,
                 const QSize& size,
                 std::size_t componentCount,
                 bool copyData,
                 QObject* parent)
  : QObject(parent),
    m_serial(generateSerial()),
    m_noReconcile(0),
    m_isValid(false),
    m_componentType(imageType),
    m_size(size),
    m_componentCount(componentCount),
    m_byteCount(0)
{
    init(const_cast<RawData&>(rawData), copyData);
}

SSImage::SSImage(const SSImage &rhs, bool copyData)
  : QObject(rhs.parent()),
    m_serial(rhs.m_serial),
    m_noReconcile(0),
    m_isValid(false),
    m_componentType(rhs.m_componentType),
    m_size(rhs.m_size),
    m_componentCount(rhs.m_componentCount),
    m_byteCount(rhs.m_byteCount)
{
    init(const_cast<RawData&>(rhs.m_rawData), copyData);
}

SSImage::~SSImage()
{
}

void SSImage::init(RawData& rawData, bool copyData)
{
    if(m_componentCount <= 0)
        throw std::invalid_argument("The value supplied to full constructor for componentCount must be >= 1.");
    if(m_size.width() <= 0 || m_size.height() <= 0)
        throw std::invalid_argument("The value supplied to full constructor for size must have positive width and height.");
    m_byteCount =
        static_cast<std::size_t>(m_size.width()) *
        static_cast<std::size_t>(m_size.height()) *
        m_componentCount *
        ImageTypeSizes[m_componentType];
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
            setComponentType(rhs.m_componentType);
            setSize(rhs.m_size);
            setChannelCount(rhs.m_componentCount);
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

bool SSImage::operator == (const SSImage& rhs) const
{
    bool ret{false};
    if(this == &rhs)
    {
        ret = true;
    }
    else if (m_componentType == rhs.m_componentType
           && m_isValid == rhs.m_isValid
           && m_size == rhs.m_size
           && m_componentCount == rhs.m_componentCount )
    {
        if(m_isValid)
        {
            if(m_componentType == UInt12DT)
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

QImage SSImage::as10BpcQImage() const
{
    if(!m_isValid) return QImage();
    const std::uint8_t* fiPixIt{reinterpret_cast<const std::uint8_t*>(m_rawData.get())};
    const std::uint8_t* const fiPixEndIt{fiPixIt + m_byteCount};
    std::uint8_t* qiPixIt;
    QImage ret;
    float aFactor;
    switch(m_componentCount)
    {
    default:
        qWarning("m_componentCount must be in the interval [0, 4].");
        break;
    case 4:
    {
        ret = QImage(m_size, QImage::Format_A2RGB30_Premultiplied);
        qiPixIt = ret.bits();
        for(;;)
        {
            const FIRGBA16& fiPix = *reinterpret_cast<const FIRGBA16*>(fiPixIt);
            aFactor = static_cast<float>(fiPix.alpha) / 0xffff;
            // Should be endian safe (not tested on big endian, however)
            *reinterpret_cast<std::uint32_t*>(qiPixIt) =
                ((static_cast<std::uint32_t>(fiPix.alpha          ) >> 14) << 30) |
                ((static_cast<std::uint32_t>(fiPix.red   * aFactor) >>  6) << 20) |
                ((static_cast<std::uint32_t>(fiPix.green * aFactor) >>  6) << 10) |
                 (static_cast<std::uint32_t>(fiPix.blue  * aFactor) >>  6);
            fiPixIt += 8;
            if(fiPixIt >= fiPixEndIt) break;
            qiPixIt += 4;
        }
        break;
//    }
//    case 3:
//    {
//        // TODO: verify this block
//        struct FiPixel
//        {
//            uint16_t r : 16;
//            uint16_t g : 16;
//            uint16_t b : 16;
//        };
//        ret = QImage(m_size, QImage::QImage::Format_RGB30);
//        qiPixIt = ret.bits();
//        for(;;)
//        {
//            const FiPixel& fiPix = *reinterpret_cast<const FiPixel*>(fiPixIt);
////            QiPixel& qiPix = *reinterpret_cast<QiPixel*>(qiPixIt);
////            qiPix.r = fiPix.r >> 6;
////            qiPix.g = fiPix.g >> 6;
////            qiPix.b = fiPix.b >> 6;
//            fiPixIt += 6;
//            if(fiPixIt >= fiPixEndIt) break;
//            qiPixIt += 4;
//        }
//        break;
//    }
//    case 2:
//    {
//        // TODO
//        qWarning("Support for m_componentCount of 2 is not yet implemented.");
//        break;
//    }
//    case 1:
//    {
        // Note: QImage does not offer a 10-bit grayscale format
//        ret = QImage(m_size, QImage::QImage::Format_RGB30);
//        qiPixIt = ret.bits();
//        for(;;)
//        {
//            const uint16_t& fiPix = *reinterpret_cast<const uint16_t*>(fiPixIt);
//            QiPixel& qiPix = *reinterpret_cast<QiPixel*>(qiPixIt);
//            qiPix.b = qiPix.g = qiPix.r = fiPix >> 6;
//            fiPixIt += 2;
//            if(fiPixIt >= fiPixEndIt) break;
//            qiPixIt += 4;
//        }
//        break;
    }}
    return ret;
}

std::size_t SSImage::serial() const
{
    return m_serial;
}

bool SSImage::isValid() const
{
    return m_isValid;
}

SSImage::DType SSImage::componentType() const
{
    return m_componentType;
}

void SSImage::setComponentType(DType componentType)
{
    if(componentType != m_componentType)
    {
        m_componentType = componentType;
        imageTypeChanged(m_componentType);
    }
    reconcile();
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
        sizeChanged(m_size);
    }
    reconcile();
}

std::size_t SSImage::componentCount() const
{
    return m_componentCount;
}

void SSImage::setChannelCount(std::size_t componentCount)
{
    if(componentCount != m_componentCount)
    {
        m_componentCount = componentCount;
        componentCountChanged(m_componentCount);
    }
    reconcile();
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
            std::size_t componentCount;
            switch(fiImage.getImageType())
            {
            default:
                componentCount = 0;
                break;
            case FIT_BITMAP:
                switch(fiImage.getBitsPerPixel())
                {
                default:
                    componentCount = 1;
                    break;
                case 24:
                case 32:
                    componentCount = 4;
                    break;
                }
                break;
            case FIT_UINT16:
            case FIT_INT16:
            case FIT_UINT32:
            case FIT_INT32:
            case FIT_FLOAT:
            case FIT_DOUBLE:
            case FIT_RGB16:
            case FIT_RGBA16:
            case FIT_RGBF:
            case FIT_RGBAF:
                componentCount = 4;
                break;
            }
            if(componentCount == 0)
            {
                qDebug("Image::read(..): Unsupported data type.");
            }
            else if(componentCount == 1)
            {
                if(!fiImage.convertToUINT16())
                {
                    qDebug("Image::read(..): Failed to convert image to uint16 grayscale.");
                    componentCount = 0;
                }
            }
            else if(componentCount == 4)
            {
                if(!fiImage.convertToRGBA16())
                {
                    qDebug("Image::read(..): Failed to convert image to uint16 RGBA.");
                    componentCount = 0;
                }
            }
            if(componentCount > 0)
            {
                std::size_t oldByteCount{m_byteCount};
                {
                    NoReconcile NR(*this);
                    setComponentType(UInt16DT);
                    setSize(QSize(static_cast<int>(fiImage.getWidth()), static_cast<int>(fiImage.getHeight())));
                    setChannelCount(componentCount);
                    m_byteCount = m_componentCount * fiImage.getWidth() * fiImage.getHeight() * ImageTypeSizes[m_componentType];
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
            m_componentType != NullDT &&
            m_size.width() >= 1 && m_size.height() >= 1 &&
            m_componentCount >= 1
        };
        if(valid != m_isValid)
        {
            if(valid)
            {
                std::size_t byteCount =
                    static_cast<std::size_t>(m_size.width()) *
                    static_cast<std::size_t>(m_size.height()) *
                    m_componentCount *
                    ImageTypeSizes[m_componentType];
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
