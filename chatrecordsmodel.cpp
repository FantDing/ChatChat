#include "chatrecordsmodel.h"

ChatRecordsModel::ChatRecordsModel(QObject *parent) : QAbstractListModel(parent)
{
    pushBack("welcome & say hello");
}

void ChatRecordsModel::pushBack(QString content)
{
    beginInsertColumns(QModelIndex(),rowCount(),rowCount());
    ChatRecordItem item=ChatRecordItem(content);
    items.push_back(item);
    endInsertRows();
}

ChatRecordItem::ChatRecordItem(QString content)
{
    this->content=content;
}

QString ChatRecordItem::getContent() const
{
    return content;
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
    return QVariant();
}

QHash<int, QByteArray> ChatRecordsModel::roleNames() const
{
    QHash<int,QByteArray> hash;
    hash[Role::role1]="content";
    return hash;
}
