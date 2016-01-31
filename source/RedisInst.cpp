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

static const char* s_redisPaths[] =
{
    "redis-server",
#ifdef Q_OS_UNIX
    "/usr/local/bin/redis-server",
    "/opt/bin/redis-server",
    "/sw/local/bin/redis-server",
#elif Q_OS_WIN
    // TODO: Update readme.md to make clear that windows users need to swiper the Zune charm to
    // install a two-tone metro design language version of redis from the XBOX store or whatever.
    "C:/program files/redis/redis-server",
    "C:/program files (x86)/redis/redis-server",
#endif
};

RedisInst::RedisInst(QObject *parent)
  : QObject(parent),
    m_ok(false)
{
    QStringList args; args << "-";
    for(const char** redisPath{s_redisPaths}, **redisPathsEnd{s_redisPaths + sizeof(s_redisPaths)/sizeof(char*)};
        redisPath != redisPathsEnd;
        ++redisPath)
    {
        m_process = new QProcess(this);
        m_process->start(*redisPath, args);
        m_process->write("daemonize no                                      \n"
                         "port 65445                                        \n"
                         "tcp-backlog 511                                   \n"
                         "bind 127.0.0.1                                    \n"
                         "timeout 0                                         \n"
                         "tcp-keepalive 60                                  \n"
                         "loglevel notice                                   \n"
                         "logfile \"\"                                      \n"
                         "databases 16                                      \n"
                         "slave-serve-stale-data yes                        \n"
                         "slave-read-only yes                               \n"
                         "repl-diskless-sync no                             \n"
                         "repl-diskless-sync-delay 5                        \n"
                         "repl-disable-tcp-nodelay no                       \n"
                         "slave-priority 100                                \n"
                         "appendonly no                                     \n"
                         "lua-time-limit 5000                               \n"
                         "slowlog-log-slower-than 10000                     \n"
                         "slowlog-max-len 128                               \n"
                         "latency-monitor-threshold 0                       \n"
                         "notify-keyspace-events \"\"                       \n"
                         "hash-max-ziplist-entries 512                      \n"
                         "hash-max-ziplist-value 64                         \n"
                         "list-max-ziplist-entries 512                      \n"
                         "list-max-ziplist-value 64                         \n"
                         "set-max-intset-entries 512                        \n"
                         "zset-max-ziplist-entries 128                      \n"
                         "zset-max-ziplist-value 64                         \n"
                         "hll-sparse-max-bytes 3000                         \n"
                         "activerehashing yes                               \n"
                         "client-output-buffer-limit normal 0 0 0           \n"
                         "client-output-buffer-limit slave 256mb 64mb 60    \n"
                         "client-output-buffer-limit pubsub 32mb 8mb 60     \n"
                         "hz 10                                             \n"
                         "aof-rewrite-incremental-fsync yes                 \n");
        m_process->closeWriteChannel();
        if(m_process->waitForStarted(3000))
        {
            m_ok = true;
            break;
        }
        else
        {
            delete m_process;
            m_process = nullptr;
        }
    }
    qDebug() << "redis started: " << m_ok;
}

RedisInst::~RedisInst()
{
    if(m_process)
    {
        m_process->terminate();
        m_process->waitForFinished();
    }
}

RedisInst::operator bool() const
{
    return m_ok;
}
