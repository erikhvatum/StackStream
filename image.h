#pragma once

#include <QtGui>
#include <cstdint>
#include <memory>

class Image
  : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool is_null READ is_null CONSTANT STORED false)
    Q_PROPERTY(QSize size MEMBER size CONSTANT)
    Q_PROPERTY(std::size_t channel_count MEMBER channel_count CONSTANT)
    Q_PROPERTY(std::size_t byte_count MEMBER byte_count CONSTANT)
public:
    enum ImageType
    {
        NullImage = 0,
        uint8Image,
        uint12Image,
        uint16Image,
        float32Image
    };
    static const std::size_t ImageTypeSizes[];
    Q_ENUM(ImageType)
    Q_PROPERTY(ImageType image_type MEMBER image_type CONSTANT)

    const ImageType image_type;
    const std::shared_ptr<std::uint8_t> data;
    const QSize size;
    const std::size_t channel_count;
    const std::size_t byte_count;

    Image(QObject* parent=nullptr);
    explicit Image(ImageType _image_type,
                   const std::shared_ptr<std::uint8_t>& _data,
                   const QSize& _size,
                   std::size_t _channel_count=1,
                   bool copy_data=true,
                   QObject* parent=nullptr);
    // If true is supplied for copy_data, the copy constructor copies contents of rhs.data (and therefore, in this case,
    // does not increase rhs.m_data's reference count).
    Image(const Image& rhs, bool copy_data=false);
    virtual ~Image();
    Image& operator = (const Image& rhs) = delete;

    bool operator == (const Image& rhs) const;
    operator bool () const;
    bool is_null() const;

signals:
    void data_changed();

public slots:
};
