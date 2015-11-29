#include "common.h"
#include <exception>
#include <cstring>

#include "Image.h"

static void noop_deleter(void*) {}
using get_default_deleter = std::default_delete<std::uint8_t[]>;

class NoReconcile
{
public:
    explicit NoReconcile(Image& image) : m_image(image)
    {
        ++m_image.m_noReconcile;
    }
    ~NoReconcile()
    {
        --m_image.m_noReconcile;
    }
private:
    Image& m_image;
};

const std::size_t Image::ImageTypeSizes[] =
{
    0, // NullImage
    1, // uint8Image
    2, // uint12Image
    2, // uint16Image
    4, // uint32Image
    8, // uint64Image
    4, // float32Image
    8, // float64Image
};

Image::Image(QObject *parent)
  : QObject(parent),
    m_noReconcile(false),
    m_isValid(false),
    m_imageType(NullImage),
    m_channelCount(0),
    m_byteCount(0)
{
}

Image::Image(ImageType imageType, const QSize& size, std::size_t channelCount, QObject* parent)
  : QObject(parent),
    m_noReconcile(false),
    m_isValid(false),
    m_imageType(imageType),
    m_size(size),
    m_channelCount(channelCount),
    m_byteCount(0)
{
    reconcile();
}

Image::Image(ImageType imageType,
             const std::uint8_t* rawData,
             const QSize& size,
             std::size_t channelCount,
             QObject* parent)
  : QObject(parent),
    m_noReconcile(false),
    m_imageType(imageType),
    m_size(size),
    m_channelCount(channelCount),
    m_byteCount(0)
{
    RawData _rawData{const_cast<std::uint8_t*>(rawData), noop_deleter};
    init(_rawData, true);
}

Image::Image(ImageType imageType,
             std::uint8_t* rawData,
             const QSize& size,
             std::size_t channelCount,
             bool takeOwnership,
             QObject* parent)
  : QObject(parent),
    m_noReconcile(false),
    m_isValid(false),
    m_imageType(imageType),
    m_size(size),
    m_channelCount(channelCount)
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

Image::Image(ImageType imageType,
             const RawData& rawData,
             const QSize& size,
             std::size_t channelCount,
             bool copyData,
             QObject* parent)
  : QObject(parent),
    m_noReconcile(false),
    m_isValid(false),
    m_imageType(imageType),
    m_size(size),
    m_channelCount(channelCount),
    m_byteCount(0)
{
    init(const_cast<RawData&>(rawData), copyData);
}

Image::Image(const Image &rhs, bool copyData)
  : QObject(rhs.parent()),
    m_noReconcile(false),
    m_isValid(false),
    m_imageType(rhs.m_imageType),
    m_size(rhs.m_size),
    m_channelCount(rhs.m_channelCount),
    m_byteCount(rhs.m_byteCount)
{
    init(const_cast<RawData&>(rhs.m_rawData), copyData);
}

void Image::init(RawData& rawData, bool copyData)
{
    if(m_channelCount <= 0)
        throw std::invalid_argument("The value supplied to full constructor for channelCount must be >= 1.");
    if(m_size.width() <= 0 || m_size.height() <= 0)
        throw std::invalid_argument("The value supplied to full constructor for size must have positive width and height.");
    m_byteCount =
        static_cast<std::size_t>(m_size.width()) *
        static_cast<std::size_t>(m_size.height()) *
        m_channelCount *
        ImageTypeSizes[m_imageType];
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

Image& Image::operator = (const Image& rhs)
{
    if(this != &rhs)
    {
        {
            NoReconcile NR(*this);
            setImageType(rhs.m_imageType);
            setSize(rhs.m_size);
            setChannelCount(rhs.m_channelCount);
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
            dataChanged();
        }
        else
        {
            reconcile();
        }
    }
    return *this;
}

bool Image::operator == (const Image& rhs) const
{
    bool ret{false};
    if(this == &rhs)
    {
        ret = true;
    }
    else if ( m_imageType == rhs.m_imageType
           && m_isValid == rhs.m_isValid
           && m_size == rhs.m_size
           && m_channelCount == rhs.m_channelCount )
    {
        if(m_isValid)
        {
            if(m_imageType == uint12Image)
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
                ret = std::memcmp(m_rawData.get(), rhs.m_rawData.get(), m_byteCount);
            }
        }
        else
        {
            ret = true;
        }
    }
    return ret;
}

Image::operator bool () const
{
    return isValid();
}

bool Image::isValid() const
{
    return m_isValid;
}

Image::ImageType Image::imageType() const
{
    return m_imageType;
}

void Image::setImageType(ImageType imageType)
{
    if(imageType != m_imageType)
    {
        m_imageType = imageType;
        imageTypeChanged(m_imageType);
    }
    reconcile();
}

const Image::RawData& Image::rawData() const
{
    return m_rawData;
}

const QSize& Image::size() const
{
    return m_size;
}

void Image::setSize(const QSize& size)
{
    if(size != m_size)
    {
        m_size = size;
        sizeChanged(m_size);
    }
    reconcile();
}

std::size_t Image::channelCount() const
{
    return m_channelCount;
}

void Image::setChannelCount(std::size_t channelCount)
{
    if(channelCount != m_channelCount)
    {
        m_channelCount = channelCount;
        channelCountChanged(m_channelCount);
    }
    reconcile();
}

std::size_t Image::byteCount() const
{
    return m_byteCount;
}

bool Image::read(const QUrl& furl)
{
    bool ret{false};
    if(!furl.isLocalFile())
    {
        qDebug("Image::read(..): Only file:// URLs are supported at the moment.");
    }
    else
    {
        fipImage fiImage;
        QString fpath{furl.toLocalFile()};
        if(fiImage.load(reinterpret_cast<const char*>(fpath.unicode())))
        {
            BITMAPINFOHEADER* header{fiImage.getInfoHeader()};
            std::size_t channelCount = 0;
            switch(header->biPlanes)
            {
            case 1:
                if(!fiImage.convertToUINT16())
                    qDebug("Image::read(..): Failed to convert image to uint16 grayscale.");
                else
                    channelCount = 1;
                break;
            case 3:
            case 4:
                if(!fiImage.convertToRGBA16())
                    qDebug("Image::read(..): Failed to convert image to uint16 RGBA.");
                else
                    channelCount = 4;
                break;
            default:
                qDebug("Image::read(..): Unsupported channel count.");
            }
            if(channelCount > 0)
            {
                std::size_t oldByteCount{m_byteCount};
                {
                    NoReconcile NR(*this);
                    setImageType(uint16Image);
                    setSize(QSize(static_cast<int>(fiImage.getWidth()), static_cast<int>(fiImage.getHeight())));
                    setChannelCount(channelCount);
                    m_byteCount = m_channelCount * fiImage.getWidth() * fiImage.getHeight() * ImageTypeSizes[m_imageType];
                    m_rawData.reset(new std::uint8_t[m_byteCount]);
                    std::memcpy(m_rawData.get(), fiImage.accessPixels(), m_byteCount);
                    if(m_byteCount != oldByteCount)
                        byteCountChanged(m_byteCount);
                    dataChanged();
                }
            }
        }
    }
    return ret;
}

bool Image::write(const QUrl& furl) const
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

void Image::reconcile()
{
    if(m_noReconcile == 0)
    {
        const bool valid {
            m_imageType != NullImage &&
            m_size.width() >= 1 && m_size.height() >= 1 &&
            m_channelCount >= 1
        };
        if(valid != m_isValid)
        {
            if(valid)
            {
                std::size_t byteCount =
                    static_cast<std::size_t>(m_size.width()) *
                    static_cast<std::size_t>(m_size.height()) *
                    m_channelCount *
                    ImageTypeSizes[m_imageType];
                if(byteCount != m_byteCount)
                {
                    m_byteCount = byteCount;
                    byteCountChanged(m_byteCount);
                    m_rawData.reset(new std::uint8_t[m_byteCount], get_default_deleter());
                    dataChanged();
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
                    dataChanged();
                }
            }
            isValidChanged(m_isValid);
        }
    }
}
