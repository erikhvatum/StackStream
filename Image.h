#pragma once

#include "common.h"

class Image
  : public QObject
{
public:
    enum ImageType
    {
        // TODO: change to format Type_float32
        NullImage = 0,
        uint8Image,
        uint12Image,
        uint16Image,
        float32Image
    };
    typedef std::shared_ptr<std::uint8_t> RawData;

private:
    Q_OBJECT
    Q_ENUM(ImageType)
    Q_PROPERTY(bool isNull READ isNull STORED false NOTIFY isNullChanged FINAL)
    Q_PROPERTY(bool isValid READ isValid STORED false NOTIFY isValidChanged FINAL)
    Q_PROPERTY(ImageType imageType READ imageType WRITE setImageType NOTIFY imageTypeChanged FINAL)
    Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged FINAL)
    Q_PROPERTY(std::size_t channelCount READ channelCount WRITE setChannelCount NOTIFY channelCountChanged FINAL)
    Q_PROPERTY(std::size_t byteCount READ byteCount STORED false NOTIFY byteCountChanged FINAL)

public:
    
    static const std::size_t ImageTypeSizes[];

    explicit Image(QObject* parent=nullptr);

    explicit Image(ImageType imageType, const QSize& size, std::size_t channelCount=1, QObject* parent=nullptr);

    // Copies contents of rawData to m_rawData; manages m_rawData's buffer
    Image(ImageType imageType,
          const std::uint8_t* rawData,
          const QSize& size,
          std::size_t channelCount=1,
          QObject* parent=nullptr);

    // Makes m_rawData point to the same location as rawData; manages m_rawData's buffer if takeOwnership is true
    Image(ImageType image_type,
          std::uint8_t* rawData,
          const QSize& size,
          std::size_t channelCount=1,
          bool takeOwnership=false,
          QObject* parent=nullptr);

    // If copyData is True: copies contents of rawData to m_rawData; manages m_rawData's buffer. 
    // If copyData is False: copies rawData to m_rawData.  Therefore, m_rawData's buffer management is controlled by the 
    // value of rawData.deleter, a function pointer copied to m_rawData.deleter. 
    Image(ImageType imageType,
          const RawData& rawData,
          const QSize& size,
          std::size_t channelCount=1,
          bool copyData=true,
          QObject* parent=nullptr);

    // If copyData is True: copies contents of rhs.m_rawData to m_rawData; manages m_rawData's buffer. 
    // If copyData is false: copies rhs.m_rawData to m_rawData.  Therefore, m_rawData's buffer management is controlled 
    // by the value of rhs.m_rawData.deleter, a function pointer copied to m_rawData.deleter.
    Image(const Image& rhs, bool copyData=false);

    // rhs.m_rawData is copied (IE, the smart pointer itself is copied and not the data it points to) iff rhs is valid
    Image& operator = (const Image& rhs);

    virtual ~Image();

    bool operator == (const Image& rhs) const;
    operator bool () const;

    bool isNull() const;

    bool isValid() const;

    ImageType imageType() const;
    void setImageType(ImageType imageType);

    const RawData& rawData() const;

    const QSize& size() const;
    void setSize(const QSize& size);

    std::size_t channelCount() const;
    void setChannelCount(std::size_t channelCount);

    std::size_t byteCount() const;

signals:
    void isNullChanged(bool);
    void isValidChanged(bool);
    void imageTypeChanged(ImageType);
    void sizeChanged(QSize);
    void channelCountChanged(std::size_t);
    void byteCountChanged(std::size_t);
    void dataChanged();

public slots:

protected:
    bool m_isValid;
    ImageType m_imageType;
    RawData m_rawData;
    QSize m_size;
    std::size_t m_channelCount;
    std::size_t m_byteCount;

    void init(std::shared_ptr<std::uint8_t>& rawData, bool copyData);
    void updateValidity();
};
