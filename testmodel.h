#pragma once

#include <QtCore>
#include "layer.h"

class TestModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QList<Layer*> layers MEMBER m_layers NOTIFY layersChanged)
public:
    TestModel();
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
//    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    Q_INVOKABLE void setProperty(int row, const QVariant& value, const QString& role_name);
//    Qt::ItemFlags flags(const QModelIndex & index) const;
    QHash<int, QByteArray> roleNames() const;

signals:
    void layersChanged();

public:
    QList<Layer*> m_layers;

protected:
    QHash<int, QByteArray> m_role_names;
};
