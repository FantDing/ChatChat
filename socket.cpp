#include "socket.h"
Socket::Socket()
{
    this->friendsModel=new FriendsModel();
    this->friendsModel->pushBack("192.168.2.2","zhugeliang");
    this->friendsModel->pushBack("192.168.3.2","liubei");
    this->friendsModel->pushBack("192.168.4.2","guanyu");
    this->friendsModel->pushBack("192.168.5.2","zhangfei");
    this->friendsModel->pushBack("192.168.6.2","caocao");
    this->friendsModel->pushBack("192.168.7.2","sunquan");
}

FriendsModel *Socket::getFriendsModel() const
{
    return friendsModel;
}

ChatRecordsModel *Socket::getChatModel()
{
    bool b_contain=map.contains(this->currentFriend);
    if(b_contain){
        return map[this->currentFriend];
    }
    else{
        ChatRecordsModel* ptr=new ChatRecordsModel();
        map[this->currentFriend]=ptr;
        return ptr;
    }
}

void Socket::setCurrentFriend(const QString &value)
{
    currentFriend = value;
}
