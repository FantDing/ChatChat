#include "chatrecordsmodel.h"

ChatRecordsModel::ChatRecordsModel(QObject *parent) : QAbstractListModel(parent)
{
    pushBack("welcome & say hello",false);
}

void ChatRecordsModel::pushBack(QString content,bool direction)
{
    beginInsertColumns(QModelIndex(),rowCount(),rowCount());
    ChatRecordItem item=ChatRecordItem(content,direction);
    items.push_back(item);
    endInsertRows();
}

void ChatRecordsModel::clear()
{
    beginRemoveRows(QModelIndex(),0,items.size());
    items.clear();
    endRemoveRows();
}

//ChatRecordItem::ChatRecordItem(QString content)
//{
//    this->content=content;
//}

ChatRecordItem::ChatRecordItem(QString content, bool direction)
{
    this->content=content;
    this->direction=direction;
}

QString ChatRecordItem::getContent() const
{
    return content;
}

bool ChatRecordItem::getDirection() const
{
    return direction;
}


int ChatRecordsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return this->items.size();
}

QVariant ChatRecordsModel::data(const QModelIndex &index, int role) const
{
    int row=index.row();
    ChatRecordItem item=items[row];
    if(role==Role::role1){
        return item.getContent();
    }
    else if(role==Role::role2){
        return item.getDirection();
    }
    return QVariant();
}

QHash<int, QByteArray> ChatRecordsModel::roleNames() const
{
    QHash<int,QByteArray> hash;
    hash[Role::role1]="content";
    hash[Role::role2]="direction";
    return hash;
}
