#include "socket.h"
#include<QDebug>
#include<QNetworkInterface>
#include<QByteArray>
#include<QDataStream>

Socket::Socket()
{
    this->friendsModel=new FriendsModel();
    this->udpSocket=NULL;
    //tcp
    bytesWrriten=0;
    payloadSize=0;
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
    out<<type<<nickName<<ipv4;// Type MyName MyIP
    switch (type) {
    case HELLO:// HELLO MyName MyIP
        udpSocket->writeDatagram(data,data.length(),targetAddress,udpPort);
        break;
    case SAY:
        // SAY MyName MyIp FriendName content
        out<<friendName<<content;
        udpSocket->writeDatagram(data,data.length(),targetAddress,udpPort);
        break;
    case FILECOME:// FILECOME MyName MyIP FriendName FileName
        out<<friendName<<content;
        qDebug()<<"FILECOME "<<nickName<<" "<<ipv4<<" "<<friendName<<" "<<content;
        udpSocket->writeDatagram(data,data.length(),targetAddress,udpPort);
        break;
    case FILEREFUSE:
        udpSocket->writeDatagram(data,data.length(),targetAddress,udpPort);
        break;
    default:
        qDebug()<<"wrong proctol";
        break;
    }
}

void Socket::initalizeTcp()
{
    tcpPort=34234;
    this->tcpServer=new QTcpServer(this);
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(sendFile()));
    if(!tcpServer->listen(QHostAddress::AnyIPv4,tcpPort)){
        qDebug()<<"TCP LISTEN ERROR";
        return;
    }
    else {
        qDebug()<<"TCP listening...";
    }
    
}

void Socket::setFileName(const QString &value)
{
    fileName = value;
    if(!fileName.isEmpty()){
        theFileName=fileName.right(fileName.size()-fileName.lastIndexOf('/')-1);
    }
    qDebug()<<theFileName;
}

void Socket::handleComingDatagrams()
{
    while(udpSocket->hasPendingDatagrams()){
        QByteArray comingData;//recived
        comingData.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(comingData.data(),comingData.size());
        QDataStream in(&comingData,QIODevice::ReadOnly);
        int msgType;
        QString friendName;
        QString friendIpv4;
        QString chatContent="";
        QString targetName="";
        in>>msgType>>friendName>>friendIpv4;
        if(friendName==nickName&&friendIpv4==ipv4){
            return;
        }
        switch (msgType) {
        case HELLO:
            //must add this condition to avoid endless sendHello
            if(!friends.contains(friendName+friendIpv4)){
                friendsModel->pushBack(friendIpv4,friendName);
                sendMsg(HELLO,friendIpv4,"all");
            }
            break;
        case SAY:
            // FriendName content
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
            break;
        case FILECOME:// FriendName FileName
//            qDebug()<<"FILECOME";
            in>>targetName;
            if(targetName!=nickName){
                qDebug()<<targetName<<":NOT ME";
                return;
            }
            in>>chatContent;
            qDebug()<<"fileCome in c++";
            emit fileCome(friendName,friendIpv4,chatContent);
            break;
        case FILEREFUSE:
            emit fileStatus("Refuse");
        default:
            break;
        }
    }//while-end
}

void Socket::sendFile()
{
    tcpSocketSend=tcpServer->nextPendingConnection();
    connect(tcpSocketSend,SIGNAL(bytesWritten(qint64)),this,SLOT(SendContinueAndUpdateProgressBar(qint64)));
    locFile=new QFile(fileName);
    if(!locFile->open(QFile::ReadOnly)){
        qDebug()<<"file open fail";
        return ;
    }
    totalBytesToSend=locFile->size();
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_7);
    sendOut<<qint64(0)<<qint64(0)<<theFileName;
    
    this->totalBytesToSend+=outBlock.size();
    sendOut.device()->seek(0);
    sendOut<<totalBytesToSend<<qint64(outBlock.size()-sizeof(qint64)*2);
    bytesToWrite=totalBytesToSend-tcpSocketSend->write(outBlock);
    outBlock.resize(0);
}

void Socket::SendContinueAndUpdateProgressBar(qint64 numBytes)
{
    bytesWrriten+=(int)numBytes;
    if(bytesToWrite>0){
        outBlock=locFile->read(qMin(bytesToWrite,payloadSize));
        bytesToWrite-=(int)tcpSocketSend->write(outBlock);
        outBlock.resize(0);
    }
    else{
        locFile->close();
    }
    
    if(bytesWrriten==totalBytesToSend){
        locFile->close();
        tcpServer->close();
        qDebug()<<"send file end";
        emit fileStatus("Success");
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
