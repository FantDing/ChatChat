#ifndef FRIENDSMODEL_H
#define FRIENDSMODEL_H

#include<QAbstractListModel>
#include<QVector>
#include<QMap>


class FriendItem;
class FriendsModel:public QAbstractListModel
{
    Q_OBJECT
public:
    FriendsModel(QObject* parent=NULL);
    Q_INVOKABLE void pushBack(QString ipv4,QString nickName);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void remove(int index);
    
    int rowCount(const QModelIndex &parent= QModelIndex() ) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    // model is find by the Role
    enum Role{
        role1=1,
        role2,
//        role3
    };

    QVector<FriendItem> getItems() const;
    
private:
    QVector<FriendItem> items;
    
};


class FriendItem{
public:
    FriendItem(QString ip="",QString name=""){
        this->ipv4=ip;
        this->nickName=name;
//        this->chatRecordsModel=NULL;
    }
    
    bool operator ==(const FriendItem& item1){
        if(item1.ipv4==ipv4&&
                item1.nickName==nickName){
            return true;
        }
        else{
            return false;
        }
    }
    QString getIpv4() const;
    QString getNickName() const;
//    ChatRecordsModel *getChatRecordsModel() const;
//    void setChatRecordsModel(ChatRecordsModel *value);
    
private:
    //note: when add a property ,you must change data() & roleNames() functions
    QString ipv4;
    QString nickName;
    //every single friend has a chatRecordsModel
//    ChatRecordsModel* chatRecordsModel;
};

#endif // FRIENDSMODEL_H
