#ifndef SOCKET_H
#define SOCKET_H

#include<QObject>
#include"friendsmodel.h"
#include"chatrecordsmodel.h"


class ChatRecordsModel;
class FriendsModel;
class Socket:public QObject
{
    Q_PROPERTY(FriendsModel *friendModel READ getFriendsModel NOTIFY friendsModelChanged)
    Q_PROPERTY(ChatRecordsModel* chatModel READ getChatModel NOTIFY chatModelChanged)
    Q_OBJECT

public:
    Socket();
    FriendsModel *getFriendsModel() const;
    Q_INVOKABLE ChatRecordsModel* getChatModel();
    Q_INVOKABLE void setCurrentFriend(const QString &value);
    
private:
    FriendsModel* friendsModel;
    QMap<QString,ChatRecordsModel*> map;
    QString currentFriend;
signals:
    void friendsModelChanged(FriendsModel* newValue);
    void chatModelChanged(ChatRecordsModel* newValue);
};

#endif // SOCKET_H
