#include "socket.h"
#include<QDebug>
#include<QNetworkInterface>
#include<QByteArray>
#include<QDataStream>

Socket::Socket()
{
    this->friendsModel=new FriendsModel();
    this->udpSocket=NULL;
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

void Socket::initalizeNetwork()
{
    QList<QHostAddress> list=QNetworkInterface::allAddresses();
    foreach (QHostAddress addr, list) {
        if(addr.protocol()==QAbstractSocket::IPv4Protocol){
            this->ipv4=addr.toString();
        }
    }
    udpPort=44332;
    udpSocket=new QUdpSocket(this);
    bool success=udpSocket->bind(udpPort,QUdpSocket::ShareAddress);
    qDebug()<<success;
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(handleComingDatagrams()));
}

void Socket::sendMsg(int type,QString address)
{
    QHostAddress targetAddress; 
    if(address=="broadcast"){
        targetAddress=QHostAddress::Broadcast;
    }
    else{
        targetAddress=QHostAddress(address);
    }
    QByteArray data;//to send
    QDataStream out(&data,QIODevice::WriteOnly);
    out<<type<<nickName;// HELLO FantDing
    switch (type) {
    case HELLO:
        out<<ipv4;//HELLO FantDing "172.20.52.53"
        udpSocket->writeDatagram(data,data.length(),targetAddress,udpPort);
        break;
    default:
        qDebug()<<"wrong proctol";
        break;
    }
}

void Socket::handleComingDatagrams()
{
    while(udpSocket->hasPendingDatagrams()){
        QByteArray comingData;//recived
        comingData.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(comingData.data(),comingData.size());
        QDataStream in(&comingData,QIODevice::ReadOnly);
        int msgType;
        in>>msgType;//read MsgType first
        QString friendName;
        QString friendIpv4;
        switch (msgType) {
        case HELLO:// HELLO FantDing "172.20.52.53" ; HELLO has been read
            in>>friendName>>friendIpv4;
            if(friendName==nickName&&friendIpv4==ipv4){
                //is myself
                return;
            }
            //must add this to avoid endless sendHello
            if(!friends.contains(friendName+friendIpv4)){
                friendsModel->pushBack(friendIpv4,friendName);
                sendMsg(HELLO,ipv4);
            }
            break;
        default:
            break;
        }
    }
}


QString Socket::getNickName() const
{
    return nickName;
}

void Socket::setNickName(const QString &value)
{
    nickName = value;
}
