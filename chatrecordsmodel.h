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
    Q_INVOKABLE void pushBack(QString content);
    
    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;
    
    
    enum Role{
        role1=1
    };

private:
    QVector<ChatRecordItem> items;
};

class ChatRecordItem{
public:
    ChatRecordItem(QString content="");
    QString getContent() const;
private:
    QString content;
    QString direction;//1:send by myself
};


#endif // CHATRECORDSMODEL_H
