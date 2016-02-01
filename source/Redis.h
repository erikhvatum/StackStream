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
#include "StackStream.h"

class RedisInst;
class RedisCaptiveInst;
class RedisExternalInst;

////

class STACKSTREAM_DLLSPEC RedisConnection
  : public QObject
{
    Q_OBJECT
public:
    friend RedisInst;
    friend RedisCaptiveInst;
    friend RedisExternalInst;
    RedisConnection(RedisConnection&) = delete;
    RedisConnection& operator = (const RedisConnection&) = delete;
    operator bool() const;
    redisContext* c() const;

protected:
    redisContext* m_c;
    bool m_ok;

    RedisConnection(const QString& host, std::uint16_t tcpPort);
    ~RedisConnection();
};

//// 

class STACKSTREAM_DLLSPEC RedisInst
  : public QObject
{
public:
    enum Status
    {
        Initializing=0,
        Ready,
        Closed
    };

private:
    Q_OBJECT
    Q_ENUM(Status)
    Q_PROPERTY(Status status READ status STORED false NOTIFY statusChanged)
public:
    explicit RedisInst(QObject* parent=nullptr);
    virtual ~RedisInst();

    operator bool() const;
    Status status() const;
    QPointer<RedisConnection> getThreadSharedConnection();
    virtual RedisConnection* makeConnection() = 0;

signals:
    void statusChanged(Status newStatus, Status oldStatus);

protected:
    Status m_status;
    QThreadStorage<QPointer<RedisConnection>> m_threadSharedConnections;
};

//// 
 
class STACKSTREAM_DLLSPEC RedisCaptiveInst
  : public RedisInst
{
    Q_OBJECT
public:
    explicit RedisCaptiveInst(std::uint16_t tcpPort=0, QObject* parent=nullptr);
    virtual ~RedisCaptiveInst();

    RedisConnection* makeConnection() override;

protected:
    const std::uint16_t m_tcpPort;
    QProcess* m_process;
    QString m_stdOutBuff;
    QString m_stdErrBuff;

protected slots:
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();
    void onFinished();
};

////

class STACKSTREAM_DLLSPEC RedisExternalInst
  : public RedisInst
{
    Q_OBJECT
public:
    explicit RedisExternalInst(const QString& host, std::uint16_t tcpPort, QObject* parent=nullptr);
    virtual ~RedisExternalInst();

    RedisConnection* makeConnection() override;

protected:
    const QString m_host;
    const std::uint16_t m_tcpPort;
};
