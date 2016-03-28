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

#include "qmlplugin.h"
#include "include/QQuickPolygon"
#include "include/QQmlSvgIconHelper"
#include "include/QQmlObjectListModel"
#include "include/QQmlVariantListModel"

QtQmlTricksPlugin::QtQmlTricksPlugin(QObject* parent)
  : QQmlExtensionPlugin(parent)
{
    qDebug() << "QtQmlTricksPlugin loaded";
}

void QtQmlTricksPlugin::registerTypes(const char* uri)
{
    Q_ASSERT(uri == QLatin1String("QtQmlTricks"));
    qDebug() << "QtQmlTricksPlugin::registerTypes(..)";


//  Q_INIT_RESOURCE (components);

//    const char * uri = "QtQmlTricks"; // @uri QtQmlTricks
    const int    maj = 1;
    const int    min = 0;

    qmlRegisterType<QQuickPolygon>     (uri, maj, min, "Polygon");
    qmlRegisterType<QQmlSvgIconHelper> (uri, maj, min, "SvgIconHelper");

    qmlRegisterUncreatableType<QAbstractItemModel>   (uri, maj, min, "AbstractItemModel", "!!!");
    qmlRegisterUncreatableType<QAbstractListModel>   (uri, maj, min, "AbstractListModel", "!!!");
    qmlRegisterUncreatableType<QQmlVariantListModel> (uri, maj, min, "VariantListModel",  "!!!");
    qmlRegisterUncreatableType<QQmlObjectListModelBase> (uri, maj, min, "ObjectListModelBase",  "!!!");

//    if (engine) {
//        engine->addImportPath ("qrc:/import");
//    }
}
