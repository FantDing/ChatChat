#include "robot.h"
#include<QJsonDocument>
#include<QNetworkReply>
Robot::Robot(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    baseUrl="http://www.tuling123.com/openapi/api";
    url.setUrl(baseUrl);
    json.insert("key","2886d0c488574d989920eb6767366287");
    json.insert("userid","152423");
    request=new QNetworkRequest();
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request->setUrl(url);
}

void Robot::post(QString info)
{
    json["info"]=info;
    manager->post(*request,QJsonDocument(json).toJson());
}

QString Robot::getResponse() const
{
    return response;
}

QNetworkAccessManager *Robot::getManager() const
{
    return manager;
}

void Robot::replyFinished(QNetworkReply *reply)
{
        // 获取响应信息
        QByteArray bytes = reply->readAll();
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(bytes, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            qDebug() << QStringLiteral("fail to read json");
            return;
        }
        // 解析Json
        if (doucment.isObject()) {
            QJsonObject obj = doucment.object();
            QJsonValue value;
            if (obj.contains("text")) {
                value = obj.take("text");
                if (value.isString()) {
                    this->response= value.toString();
                }
            }
        }
}
