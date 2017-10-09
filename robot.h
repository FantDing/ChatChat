#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>
#include<QNetworkAccessManager>
#include<QJsonObject>

class Robot : public QObject
{
    Q_OBJECT
public:
    explicit Robot(QObject *parent = nullptr);
    void post(QString info);
    ~Robot(){
        delete request;
        delete manager;
    }

    QString getResponse() const;
    
    
    QNetworkAccessManager *getManager() const;
    
private:
    QNetworkAccessManager *manager;
    
    
    QNetworkRequest* request;
    QString baseUrl;
    QUrl url;
    QJsonObject json;
    QString response;
signals:
    
public slots:
    void replyFinished(QNetworkReply *reply);
};

#endif // ROBOT_H