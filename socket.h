#ifndef SOCKET_H
#define SOCKET_H

#include<QObject>
#include"friendsmodel.h"
#include"chatrecordsmodel.h"
#include<QUdpSocket>
#include<QList>
#include<QTcpServer>
#include<QTcpSocket>
#include<QFile>

class ChatRecordsModel;
class FriendsModel;
class Socket:public QObject
{
    Q_OBJECT
//    Q_ENUMS(MsgType)
    
    Q_PROPERTY(FriendsModel *friendModel READ getFriendsModel NOTIFY friendsModelChanged)
    Q_PROPERTY(ChatRecordsModel* chatModel READ getChatModel NOTIFY chatModelChanged)

public:
    // gui
    Socket();
    FriendsModel *getFriendsModel() const;
    Q_INVOKABLE ChatRecordsModel* getChatModel();
    Q_INVOKABLE void setCurrentFriend(const QString &value);
    QString getNickName() const;
    Q_INVOKABLE void setNickName(const QString &value);
    
    //socket
    enum MsgType{
        HELLO,//0: notify other client, I am online now;
        EXIT,//1: I left
        SAY,//2: chat message
        FILECOME,//3: send file
        FILEREFUSE,//4: refuse to receive message
    };
    Q_ENUM(MsgType)
    
    Q_INVOKABLE void initalizeNetwork();//set port,udpSocket......
    /*
     * @type MsgType,cause you cannot use Enums in qml,so it's a int type
     * @address target client's ipv4,also you can use "boradcast"
     * @content the msg you will send
    */
    Q_INVOKABLE void sendMsg(int type,QString address,QString friendName,QString content="");
    
    
    //tcp
    Q_INVOKABLE void initalizeTcp();
    Q_INVOKABLE void setFileName(const QString &value);
    Q_INVOKABLE void acceptAndConnect(QString friendIPv4);
    
private:
    // gui
    FriendsModel* friendsModel;
    QMap<QString,ChatRecordsModel*> map;
    QString currentFriend;
    QString nickName;
    
    //socket
    QUdpSocket* udpSocket;
    QString ipv4;
    qint16 udpPort;
    QList<QString> friends;
    
    //tcp
    QTcpServer* tcpServer;
    qint16 tcpPort;
    QTcpSocket* tcpSocketSend;
    QString fileName; //path + name
    QString theFileName; //just name
    QFile* locFile; //
    qint64 totalBytesToSend;
    qint64 bytesToWrite;
    qint64 bytesWrriten;
    qint64 payloadSize;
    QByteArray outBlock;
    
    
    //tcp receiver
    quint64 r_blockSize;
    QTcpSocket* tcpSocketRec;
    quint64  bytesReceived;
    
signals:
    //gui
    void friendsModelChanged(FriendsModel* newValue);
    void chatModelChanged(ChatRecordsModel* newValue);
    //tcp
    void fileCome(QString friendName,QString friendIpv4,QString filename);
    void fileStatus(QString status);
private slots:
    //socket
    void handleComingDatagrams();
    //tcp
    void sendFile();
    void SendContinueAndUpdateProgressBar(qint64 numBytes);
    void recFile();
    void printMsg(QAbstractSocket::SocketError socketError);
    
};

#endif // SOCKET_H
