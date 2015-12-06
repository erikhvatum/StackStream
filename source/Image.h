#pragma once
#include "common.h"

class NoReconcile;

class Image
  : public QObject
{
public:
    enum ComponentType
    {
        // TODO: change to format Type_float32
        NullImage = 0,
        uint8Image,
        uint12Image,
        uint16Image,
        uint32Image,
        uint64Image,
        float32Image,
        float64Image
    };
    typedef std::shared_ptr<std::uint8_t> RawData;
    friend class NoReconcile;

private:
    Q_OBJECT
    Q_ENUM(ComponentType)
    Q_PROPERTY(std::size_t serial READ serial STORED false NOTIFY serialChanged)
    Q_PROPERTY(bool isValid READ isValid STORED false NOTIFY isValidChanged FINAL)
    Q_PROPERTY(ComponentType imageType READ
                       componentType
                       WRITE
                       setComponentType
                       NOTIFY imageTypeChanged FINAL)
    Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged FINAL)
    Q_PROPERTY(std::size_t channelCount READ channelCount WRITE setChannelCount NOTIFY channelCountChanged FINAL)
    Q_PROPERTY(std::size_t byteCount READ byteCount STORED false NOTIFY byteCountChanged FINAL)

public:
    static const std::size_t ImageTypeSizes[];

    explicit Image(QObject* parent=nullptr);

    explicit Image(ComponentType imageType, const QSize& size, std::size_t channelCount=1, QObject* parent=nullptr);

    // Copies contents of rawData to m_rawData; manages m_rawData's buffer
    Image(ComponentType imageType,
          const std::uint8_t* rawData,
          const QSize& size,
          std::size_t channelCount=1,
          QObject* parent=nullptr);

    // Makes m_rawData point to the same location as rawData; manages m_rawData's buffer if takeOwnership is true
    Image(ComponentType image_type,
          std::uint8_t* rawData,
          const QSize& size,
          std::size_t channelCount=1,
          bool takeOwnership=false,
          QObject* parent=nullptr);

    // If copyData is True: copies contents of rawData to m_rawData; manages m_rawData's buffer. 
    // If copyData is False: copies rawData to m_rawData.  Therefore, m_rawData's buffer management is controlled by the 
    // value of rawData.deleter, a function pointer copied to m_rawData.deleter. 
    Image(ComponentType imageType,
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

    bool operator == (const Image& rhs) const;
    operator bool () const;

    std::size_t serial() const;

    bool isValid() const;

    ComponentType componentType() const;
    void setComponentType(ComponentType componentType);

    const RawData& rawData() const;

    const QSize& size() const;
    void setSize(const QSize& size);

    std::size_t channelCount() const;
    void setChannelCount(std::size_t channelCount);

    std::size_t byteCount() const;

    // If you modify the memory backing an Image instance (IE, the memory pointed to by m_rawData) and want users of 
    // that Image instance to be informed, manually call the Image::notifyOfDataChange method.  Doing so will cause 
    // the Image instance's serial to update and the serialChanged signal to be emitted. 
    Q_INVOKABLE void notifyOfDataChange();

signals:
    void serialChanged(std::size_t);
    void isValidChanged(bool);
    void imageTypeChanged(ComponentType);
    void sizeChanged(QSize);
    void channelCountChanged(std::size_t);
    void byteCountChanged(std::size_t);

public slots:
    bool read(const QUrl& furl);
    bool write(const QUrl& furl) const;

protected:
    static volatile std::atomic<std::size_t> sm_nextSerial;
    static std::size_t generateSerial();

    std::size_t m_serial;
    std::uint16_t m_noReconcile;
    bool m_isValid;
    ComponentType m_componentType;
    RawData m_rawData;
    QSize m_size;
    std::size_t m_channelCount;
    std::size_t m_byteCount;

    void init(RawData& rawData, bool copyData);
    void reconcile();
};
