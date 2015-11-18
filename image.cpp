#include <exception>
#include <cstring>

#include "image.h"

const std::size_t Image::ImageTypeSizes[] =
{
    0, // NullImage
    1, // uint8Image
    2, // uint12Image
    2, // uint16Image
    2  // float32Image
};

Image::Image(QObject *parent)
  : QObject(parent),
    image_type(NullImage),
    channel_count(0),
    byte_count(0)
{
}

Image::Image(ImageType _image_type,
             const std::uint8_t* _data,
             const QSize& _size,
             std::size_t _channel_count,
             QObject* parent)
  : QObject(parent),
    image_type(_image_type),
    size(_size),
    channel_count(_channel_count),
    byte_count(0)
{
    init(std::shared_ptr<std::uint8_t>(const_cast<std::uint8_t*>(_data), [](void*){}/*noop deleter*/), true);
}

Image::Image(ImageType _image_type,
             const std::shared_ptr<std::uint8_t>& _data,
             const QSize& _size,
             std::size_t _channel_count,
             bool copy_data,
             QObject* parent)
  : QObject(parent),
    image_type(_image_type),
    size(_size),
    channel_count(_channel_count),
    byte_count(0)
{
    init(_data, copy_data);
}

void Image::init(const std::shared_ptr<std::uint8_t>& _data,
                 bool copy_data)
{
    if ( image_type == NullImage
      || size.width() <= 0
      || size.height() <= 0
      || !_data
      || channel_count <= 0 )
    {
        const_cast<ImageType&>(image_type) = NullImage;
        const_cast<std::size_t&>(channel_count) = 0;
    }
    else
    {
        const_cast<std::size_t&>(byte_count) = 
            static_cast<std::size_t>(size.width()) *
            static_cast<std::size_t>(size.height()) *
            channel_count *
            ImageTypeSizes[image_type];
        if(copy_data)
        {
            const_cast<std::shared_ptr<std::uint8_t>&>(data).reset(new std::uint8_t[byte_count], std::default_delete<std::uint8_t[]>());
            std::memcpy(data.get(), _data.get(), byte_count);
        }
        else
        {
            const_cast<std::shared_ptr<std::uint8_t>&>(data) = _data;
        }
    }
}

Image::Image(const Image &rhs, bool copy_data)
  : QObject(rhs.parent()),
    image_type(rhs.image_type),
    size(rhs.size),
    channel_count(rhs.channel_count),
    byte_count(rhs.byte_count)
{
    if(copy_data)
    {
        const_cast<std::shared_ptr<std::uint8_t>&>(data).reset(new std::uint8_t[byte_count], std::default_delete<std::uint8_t[]>());
        std::memcpy(data.get(), rhs.data.get(), byte_count);
    }
    else
    {
        const_cast<std::shared_ptr<std::uint8_t>&>(data) = rhs.data;
    }
}

Image::~Image()
{
}

bool Image::operator == (const Image& rhs) const
{
    bool ret{false};
    if(image_type == rhs.image_type)
    {
        if(image_type == NullImage)
        {
            ret = true;
        }
        else
        {
            if(image_type == uint12Image)
            {
                // Compare only lower 12 bits
                // TODO: validate this block 
                bool same{true};
                std::uint16_t const *l{(const std::uint16_t*)data.get()};
                std::uint16_t const *r{(const std::uint16_t*)rhs.data.get()};
                const std::uint16_t * const l_e{l + byte_count/2};
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
                ret = std::memcmp(data.get(), rhs.data.get(), byte_count);
            }
        }
    }
    return ret;
}

Image::operator bool () const
{
    return is_null();
}

bool Image::is_null() const
{
    return image_type == NullImage;
}
