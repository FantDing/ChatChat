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
    qDebug()<<"bind on:"<<QHostAddress::AnyIPv4;
    bool success=udpSocket->bind(QHostAddress::AnyIPv4, udpPort,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    qDebug()<<"BIND:"<<success;
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(handleComingDatagrams()));
}

void Socket::sendMsg(int type,QString address,QString friendName,QString content)
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
    out<<type<<nickName;// Type MyName
    switch (type) {
    case HELLO:
        out<<ipv4;//HELLO MyName MyIp
        qDebug()<<"HELLO "<<nickName<<" "<<ipv4;
        udpSocket->writeDatagram(data,data.length(),targetAddress,udpPort);
        break;
    case SAY:
        // SAY MyName MyIp FriendName content
        qDebug()<<"say:"<<nickName<<"+"<<targetAddress<<"+"<<content;
        out<<ipv4<<friendName<<content;
        udpSocket->writeDatagram(data,data.length(),targetAddress,udpPort);
        break;
    default:
        qDebug()<<"wrong proctol";
        break;
    }
}

void Socket::handleComingDatagrams()
{
    qDebug()<<"recive data";
    while(udpSocket->hasPendingDatagrams()){
        QByteArray comingData;//recived
        comingData.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(comingData.data(),comingData.size());
        QDataStream in(&comingData,QIODevice::ReadOnly);
        int msgType;
        in>>msgType;//read MsgType first
        QString friendName;
        QString friendIpv4;
        QString chatContent="";
        QString targetName="";
        in>>friendName>>friendIpv4;
        qDebug()<<"recived from friend:"<<friendName<<"+"<<friendIpv4;
        if(friendName==nickName&&friendIpv4==ipv4){
            qDebug()<<"ignore";
            //is myself
            return;
        }
        switch (msgType) {
        case HELLO:// HELLO FantDing "172.20.52.53" ; HELLO has been read
            //must add this to avoid endless sendHello
            if(!friends.contains(friendName+friendIpv4)){
                friendsModel->pushBack(friendIpv4,friendName);
                sendMsg(HELLO,friendIpv4,"all");
            }
            break;
        case SAY:
            // SAY FantDing "172.20.52.53" "nice to meet U"
            //            SAY MyName MyIp FriendName content
            qDebug()<<"recive say";
            in>>targetName;
            if(targetName!=nickName){
                qDebug()<<targetName<<":NOT ME";
                return;
            }
            in>>chatContent;
            if(map.contains(friendName+friendIpv4)){
                map[friendName+friendIpv4]->pushBack(chatContent,false);
            }
            else{
                map[friendName+friendIpv4]=new ChatRecordsModel();
                map[friendName+friendIpv4]->pushBack(chatContent,false);
            }
            //            map.value(friendName+friendIpv4)->pushBack(chatContent,false);
            break;
        default:
            break;
        }
    }//while-end
}


QString Socket::getNickName() const
{
    return nickName;
}

void Socket::setNickName(const QString &value)
{
    nickName = value;
}
