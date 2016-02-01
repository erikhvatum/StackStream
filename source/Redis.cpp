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

#include "StackStream.h"

RedisConnection::RedisConnection(const QString& host, std::uint16_t tcpPort)
  : m_ok(false)
{
    QByteArray host_(host.toLocal8Bit());
    m_c = redisConnectWithTimeout(host_.data(),
                                  static_cast<int>(tcpPort),
                                  {1, 500000}); // 1.5 second timeout
    if(!m_c)
    {
        qWarning("RedisConnection::RedisConnection(const QString& host, std::uint16_t tcpPort): Failed to allocate redis context.");
    }
    else if(m_c->err)
    {
        QByteArray e(QString("RedisConnection::RedisConnection(const QString& host, "
                             "std::uint16_t tcpPort): Failed to connect to redis: \"%1\".").arg(m_c->errstr).toLocal8Bit());
        qWarning(e.data());
        redisFree(m_c);
        m_c = nullptr;
    }
    else
    {
        m_ok = true;
    }
}

RedisConnection::~RedisConnection()
{
    if(m_c)
    {
        redisFree(m_c);
        m_c = nullptr;
    }
}

RedisConnection::operator bool() const
{
    return m_ok;
}

redisContext* RedisConnection::c() const
{
    return m_c;
}

RedisInst::RedisInst(QObject *parent)
  : QObject(parent),
    m_status(Initializing)
{
}

RedisInst::~RedisInst()
{
}

QPointer<RedisConnection> RedisInst::getThreadSharedConnection()
{
    QPointer<RedisConnection> ret;
    if(m_threadSharedConnections.hasLocalData())
        ret = m_threadSharedConnections.localData();
    if(ret.isNull())
    {
        ret = makeConnection();
        m_threadSharedConnections.setLocalData(ret);
    }
    return ret;
}

RedisInst::Status RedisInst::status() const
{
    return m_status;
}

RedisInst::operator bool() const
{
    return m_status == Ready;
}

static const char* s_captiveRedisPaths[] =
{
    "redis-server",
#ifdef Q_OS_UNIX
    "/usr/local/bin/redis-server",
    "/opt/bin/redis-server",
    "/sw/local/bin/redis-server",
#elif Q_OS_WIN
    // TODO: Update readme.md to make clear that windows users need to swipe the Zune charm to
    // install a two-tone metro design language version of redis from the XBOX store or whatever.
    "C:/program files/redis/redis-server",
    "C:/program files (x86)/redis/redis-server",
#endif
};

static const QString s_captiveRedisConf{
R"|||(daemonize no
tcp-backlog 511
bind 127.0.0.1
timeout 0
tcp-keepalive 60
loglevel notice
logfile ""
databases 16
slave-serve-stale-data yes
slave-read-only yes
repl-diskless-sync no
repl-diskless-sync-delay 5
repl-disable-tcp-nodelay no
slave-priority 100
appendonly no
lua-time-limit 5000
slowlog-log-slower-than 10000
slowlog-max-len 128
latency-monitor-threshold 0
notify-keyspace-events ""
hash-max-ziplist-entries 512
hash-max-ziplist-value 64
list-max-ziplist-entries 512
list-max-ziplist-value 64
set-max-intset-entries 512
zset-max-ziplist-entries 128
zset-max-ziplist-value 64
hll-sparse-max-bytes 3000
activerehashing yes
client-output-buffer-limit normal 0 0 0
client-output-buffer-limit slave 256mb 64mb 60
client-output-buffer-limit pubsub 32mb 8mb 60
hz 10
aof-rewrite-incremental-fsync yes)|||"};

RedisCaptiveInst::RedisCaptiveInst(std::uint16_t tcpPort, QObject* parent)
  : RedisInst(parent),
    m_tcpPort(tcpPort),
    m_process(nullptr)
{
    QString conf;
    QStringList args; args << "-";
    for(const char** redisPath{s_captiveRedisPaths},
                  ** redisPathsEnd{s_captiveRedisPaths + sizeof(s_captiveRedisPaths)/sizeof(char*)};
        redisPath != redisPathsEnd;
        ++redisPath)
    {
        m_process = new QProcess(this);
        m_process->start(*redisPath, args);
        conf = s_captiveRedisConf;
        if(m_tcpPort) conf += QString::number(m_tcpPort);
        m_process->write(conf.toLocal8Bit());
        m_process->closeWriteChannel();
        if(m_process->waitForStarted(3000))
        {
            connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFinished()));
            connect(m_process, &QProcess::readyReadStandardOutput, this, &RedisCaptiveInst::onReadyReadStandardOutput);
            connect(m_process, &QProcess::readyReadStandardError, this, &RedisCaptiveInst::onReadyReadStandardError);
            break;
        }
        else
        {
            delete m_process;
            m_process = nullptr;
        }
    }
    if(!m_process) qWarning("Failed to start captive redis-server instance.");
}

RedisCaptiveInst::~RedisCaptiveInst()
{
    if(m_process)
    {
        m_process->terminate();
        m_process->waitForFinished();
    }
}

RedisConnection* RedisCaptiveInst::makeConnection()
{
    return new RedisConnection("127.0.0.1", m_tcpPort);
}

static const QRegExp s_readyRegex{"[^*]+\\* The server is now ready to accept connections on port (\\d+)\\s*"};
static const QRegExp s_addrInUseRegex{"[^*]+# Creating Server TCP listening socket ([^:]+:\\d+): bind: Address already in use\\s*"};

void RedisCaptiveInst::onReadyReadStandardOutput()
{
    m_stdOutBuff += this->m_process->readAllStandardOutput();
    if(m_stdOutBuff.isEmpty()) return;
    QRegExp readyRegex{s_readyRegex};
    QRegExp addrInUseRegex{s_addrInUseRegex};
    QStringList lines(m_stdOutBuff.split('\n'));
    QChar lastLineChar{m_stdOutBuff[m_stdOutBuff.size()-1]};
    if(lastLineChar == '\n' || lastLineChar == '\r')
    {
        m_stdOutBuff.clear();
    }
    else
    {
        m_stdOutBuff = lines.back();
        lines.pop_back();
    }
    for(auto line{lines.begin()}; line != lines.end(); ++line)
    {
#ifndef QT_NO_DEBUG_OUTPUT
        qDebug() << "redis stdout: " << *line;
#endif
        if(m_status == Initializing)
        {
            if(readyRegex.exactMatch(*line))
            {
                std::uint16_t port{static_cast<std::uint16_t>(readyRegex.cap(1).toInt())};
                if(m_tcpPort != port)
                {
                    if(m_tcpPort != 0)
                        qWarning() << "Although port" << static_cast<int>(m_tcpPort)
                                   << "was specified, captive redis-server instance is listening on port" << static_cast<int>(port);
                    const_cast<std::uint16_t&>(m_tcpPort) = port;
                    m_status = Ready;
                    emit statusChanged(Ready, Initializing);
                }
            }
            else if(addrInUseRegex.exactMatch(*line))
            {
                qWarning() << "Failed to start captive redis-server instance," << addrInUseRegex.cap(1) << "is already in use.";
                m_status = Closed;
                emit statusChanged(Closed, Initializing);
            }
        }
    }
}

void RedisCaptiveInst::onReadyReadStandardError()
{
#ifndef QT_NO_DEBUG_OUTPUT
    QString str(this->m_process->readAllStandardError());
    std::string sstr{std::string("\n") + str.toStdString()};
    qDebug() << "redis stderr:" << sstr.c_str();
#endif
}

void RedisCaptiveInst::onFinished()
{
    if(m_status != Closed)
    {
        Status oldStatus = m_status;
        m_status = Closed;
        emit statusChanged(Closed, oldStatus);
    }
}

RedisExternalInst::RedisExternalInst(const QString& host, std::uint16_t tcpPort, QObject* parent)
  : RedisInst(parent),
    m_host(host),
    m_tcpPort(tcpPort)
{
}

RedisExternalInst::~RedisExternalInst()
{
}

RedisConnection* RedisExternalInst::makeConnection()
{
    return new RedisConnection(m_host, m_tcpPort);
}
