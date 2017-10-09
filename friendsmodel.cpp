#include "friendsmodel.h"
#include<QDebug>
FriendsModel::FriendsModel(QObject* parent):QAbstractListModel(parent)
{

}

void FriendsModel::pushBack(QString ipv4, QString nickName)
{
    beginInsertColumns(QModelIndex(),rowCount(),rowCount());
    FriendItem item=FriendItem(ipv4,nickName);
    items.push_back(item);
    endInsertRows();
}

void FriendsModel::clear()
{
    beginRemoveRows(QModelIndex(),0,items.size());
    items.clear();
    endRemoveRows();
}

void FriendsModel::remove(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    items.remove(index);
    endRemoveRows();
}



int FriendsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return this->items.size();
}

QVariant FriendsModel::data(const QModelIndex &index, int role) const
{
    int row=index.row();
    if(row<0||row>items.size()){
        return QVariant();
    }
    FriendItem item=items[row];
    if(role==Role::role1){
        return item.getIpv4();
    }
    else if(role==Role::role2){
        return item.getNickName();
    }
    else if(role==Role::role3){
        return item.getNewMsgCount();
    }

    return QVariant();
}

QHash<int, QByteArray> FriendsModel::roleNames() const
{
    QHash<int,QByteArray> hash;
    hash[Role::role1]="ipv4";
    hash[Role::role2]="nickName";
    hash[Role::role3]="newMsgCount";
    return hash;
}

QVector<FriendItem> FriendsModel::getItems() const
{
    return items;
}

QString FriendItem::getIpv4() const
{
    return ipv4;
}

QString FriendItem::getNickName() const
{
    return nickName;
}

int FriendItem::getNewMsgCount() const
{
    return newMsgCount;
}

void FriendItem::setNewMsgCount(const int &value)
{
    newMsgCount = value;
}
