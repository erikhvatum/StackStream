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

#pragma once
#include "common.h"

class NoReconcile;

class SSImage
  : public QObject,
    public QEnableSharedFromThis<SSImage>
{
public:
    enum DType
    {
        DTypeNull = 0,
        DTypeUInt8,
        DTypeUInt12,
        DTypeUInt16,
        DTypeUInt32,
        DTypeUInt64,
        DTypeFloat16,
        DTypeFloat32,
        DTypeFloat64
    };
    typedef std::shared_ptr<std::uint8_t> RawData;
    friend class NoReconcile;
    static const std::size_t DTypeSizes[];
    enum Components
    {
        ComponentsNull = 0,
        ComponentsGr,
        ComponentsGrA,
        ComponentsRGB,
        ComponentsRGBA,
        Components_last = ComponentsRGBA
    };
    static const QList<QStringList> smc_componentNames;
    static const std::uint8_t smc_componentsCounts[Components_last + 1];
    static const QStringList smc_componentsStrs;

private:
    Q_OBJECT
    Q_ENUM(DType)
    Q_ENUM(Components)
    Q_PROPERTY(std::size_t serial READ serial STORED false NOTIFY serialChanged)
    Q_PROPERTY(bool isValid READ isValid STORED false NOTIFY isValidChanged FINAL)
    Q_PROPERTY(DType componentDType READ componentDType WRITE setComponentDType NOTIFY componentDTypeChanged FINAL)
    Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged FINAL)
    Q_PROPERTY(Components components READ components WRITE setComponents NOTIFY componentsChanged FINAL)
    Q_PROPERTY(std::size_t byteCount READ byteCount STORED false NOTIFY byteCountChanged FINAL)

public:
    explicit SSImage(QObject* parent=nullptr);
    SSImage(DType componentDType, const QSize& size, Components components=ComponentsNull, QObject* parent=nullptr);

    // Copies contents of rawData to m_rawData; manages m_rawData's buffer
    SSImage(DType componentDType,
            const std::uint8_t* rawData,
            const QSize& size,
            Components components=ComponentsGr,
            QObject* parent=nullptr);

    // Makes m_rawData point to the same location as rawData; manages m_rawData's buffer if takeOwnership is true
    SSImage(DType componentDType,
            std::uint8_t* rawData,
            const QSize& size,
            Components components=ComponentsGr,
            bool takeOwnership=false,
            QObject* parent=nullptr);

    // If copyData is True: copies contents of rawData to m_rawData; manages m_rawData's buffer. 
    // If copyData is False: copies rawData to m_rawData.  Therefore, m_rawData's buffer management is controlled by the 
    // value of rawData.deleter, a function pointer copied to m_rawData.deleter. 
    SSImage(DType componentDType,
            const RawData& rawData,
            const QSize& size,
            Components components=ComponentsGr,
            bool copyData=true,
            QObject* parent=nullptr);

    // If copyData is True: copies contents of rhs.m_rawData to m_rawData; manages m_rawData's buffer. 
    // If copyData is false: copies rhs.m_rawData to m_rawData.  Therefore, m_rawData's buffer management is controlled 
    // by the value of rhs.m_rawData.deleter, a function pointer copied to m_rawData.deleter.
    SSImage(const SSImage& rhs, bool copyData=false);

    virtual ~SSImage();

    void clear();

    // rhs.m_rawData is copied (IE, the smart pointer itself is copied and not the data it points to) iff rhs is valid
    SSImage& operator = (const SSImage& rhs);

    bool operator == (const SSImage& rhs) const;
    operator bool () const;

    std::size_t serial() const;

    bool isValid() const;

    DType componentDType() const;
    void setComponentDType(DType componentDType);

    const RawData& rawData() const;

    const QSize& size() const;
    void setSize(const QSize& size);

    Components components() const;
    void setComponents(Components components);
    inline int componentCount() const { return smc_componentsCounts[m_components]; }

    bool hasAlphaChannel() const;

    std::size_t byteCount() const;

    inline const QStringList& componentNames() const { return smc_componentNames[componentCount()]; }
    inline const QString& componentsStr() const { return smc_componentsStrs[componentCount()]; }

    // If you modify the memory backing an Image instance (IE, the memory pointed to by m_rawData) and want users of 
    // that Image instance to be informed, manually call the Image::notifyOfDataChange method.  Doing so will cause 
    // the Image instance's serial to update and the serialChanged signal to be emitted. 
    Q_INVOKABLE void notifyOfDataChange();

signals:
    void serialChanged(std::size_t);
    void isValidChanged(bool);
    void componentDTypeChanged(DType);
    void sizeChanged(QSize);
    void componentsChanged(Components);
    void componentCountChanged(std::uint8_t);
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
    DType m_componentDType;
    Components m_components;
    QSize m_size;
    std::size_t m_byteCount;
    RawData m_rawData;

    void init(RawData& rawData, bool copyData);
    void reconcile();
};
