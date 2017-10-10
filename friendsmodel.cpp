#include "friendsmodel.h"
#include<QDebug>
FriendsModel::FriendsModel(QObject* parent):QAbstractListModel(parent)
{
    pushBack("","robot");
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

void FriendsModel::clearNewMsgCount(int index)
{
    items[index].setNewMsgCount(0);
    //通知view 从(index,0)到(index,0)发生了改变,需要刷新
    emit dataChanged(QAbstractItemModel::createIndex(index,0),QAbstractItemModel::createIndex(index,0));
}

void FriendsModel::addNewMsgCount(int index)
{
    int count=items[index].getNewMsgCount();
    items[index].setNewMsgCount(count+1);
    //通知view 从(index,0)到(index,0)发生了改变,需要刷新
    emit dataChanged(QAbstractItemModel::createIndex(index,0),QAbstractItemModel::createIndex(index,0));
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


//bool FriendsModel::setData(const QModelIndex &index, const QVariant &value, int role)
//{
//    qDebug()<<"setData";
//    int row=index.row();
//    if(role==Role::role3){
//        items[row].setNewMsgCount(value.toInt());
//        emit dataChanged(QModelIndex(),QModelIndex());
//        qDebug()<<items[row].getNewMsgCount();
//        return true;
//    }
//    return false;
//}