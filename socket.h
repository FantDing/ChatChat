#ifndef SOCKET_H
#define SOCKET_H

#include<QObject>
#include"friendsmodel.h"
#include"chatrecordsmodel.h"
#include<QUdpSocket>
#include<QList>

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
    };
    Q_ENUM(MsgType)
    
    Q_INVOKABLE void initalizeNetwork();//set port,udpSocket......
    /*
     *
     * @type MsgType,cause you cannot use Enums in qml,so it's a int type
     * @address target client's ipv4,also you can use "boradcast"
     * @content the msg you will send
     * 
    */
    Q_INVOKABLE void sendMsg(int type,QString address,QString friendName,QString content="");
    
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
    
signals:
    //gui
    void friendsModelChanged(FriendsModel* newValue);
    void chatModelChanged(ChatRecordsModel* newValue);
    
    //socket
private slots:
    //socket
    void handleComingDatagrams();
    
};

#endif // SOCKET_H
