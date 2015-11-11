#include "testmodel.h"

TestModel::TestModel()
{
    m_role_names[Qt::UserRole] = "gamma";
    m_role_names[Qt::UserRole + 1] = "tint";
}

int TestModel::rowCount(const QModelIndex &parent) const
{
    return m_layers.size();
}

QVariant TestModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid())
    {
        switch(role)
        {
        case Qt::UserRole:
            return QVariant(m_layers[index.row()]->m_gamma);
        case Qt::UserRole + 1:
            return QVariant(m_layers[index.row()]->m_tint);
        }
    }
    return QVariant();
}

bool TestModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    return false;
}

Qt::ItemFlags TestModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags = QAbstractListModel::flags(index);
    if(index.isValid())
    {
        flags |= Qt::ItemIsEditable;
        qDebug("Qt::ItemFlags TestModel::flags(const QModelIndex & index) const");
    }
    return flags;
}

QHash<int, QByteArray> TestModel::roleNames() const
{
    return m_role_names;
}
