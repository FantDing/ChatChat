#ifndef CHATRECORDSMODEL_H
#define CHATRECORDSMODEL_H

#include <QObject>
#include<QAbstractListModel>
#include<QVector>

class ChatRecordItem;
class ChatRecordsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ChatRecordsModel(QObject *parent = nullptr);
    Q_INVOKABLE void pushBack(QString content,bool direction);
    Q_INVOKABLE void clear();
    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;
    
    
    enum Role{
        role1=1,
        role2
    };

private:
    QVector<ChatRecordItem> items;
};

class ChatRecordItem{
public:
    ChatRecordItem(QString content="",bool direction=false);
    QString getContent() const;
    bool getDirection() const;
    
private:
    QString content;
    bool direction;//true:send; false:recived
};


#endif // CHATRECORDSMODEL_H
