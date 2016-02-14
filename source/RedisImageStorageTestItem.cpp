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

#include "RedisImageStorageTestItem.h"
#include "Redis.h"
#include "SSImage.h"

RedisImageStorageTestItem::RedisImageStorageTestItem(QQuickItem *parent)
  : QQuickItem(parent)
{
}

#include <chrono>
#include <iostream>
#include <iomanip>

bool RedisImageStorageTestItem::loadImages(const QList<QUrl> urls)
{

    RedisConnection* redis{qvariant_cast<RedisConnection*>(qmlContext(this)->contextProperty("redisConnection"))};
    SSImage im;
    int idx{0};
    bool ok;
    QByteArray r;

    foreach(auto url, urls)
    {
        if(im.read(url))
        {
            qDebug() << "read" << url;
            r = QString::number(idx).toUtf8();
            {
                TimeThisBlock t;
                freeReplyObject(redisCommand(redis->c(), "SET %s %b", r.data(), im.rawData().get(), im.byteCount()));
            }
        }
        else
        {
            qDebug() << "failed to read" << url;
        }
        ++idx;
    }

    return true;
}
