#include "yeelinkapi_v1.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QImage>

YeelinkObject::YeelinkObject(QObject *parent) :
    QObject(parent)
{

}
void YeelinkObject::init(QString deviceid,QString sensorid,QString apikey)
{
	qDebug()<<"欢迎访问： http://blog.csdn.net/tianzhihen_wq/article/details/52974107";
    m_deviceId = deviceid;
    m_sensorId = sensorid;
    m_apiKey = apikey;

    QString baseUrl = "http://api.yeelink.net/v1.0/device/"+m_deviceId+"/sensor/"+m_sensorId+"/";
    if(m_sensorType == TYPE_VALUE || m_sensorType == TYPE_BOOL ||m_sensorType == TYPE_GPS||m_sensorType == TYPE_GENERIC){
        m_downUrl = m_upUrl = QUrl(baseUrl + "datapoints");
    }
    else if(m_sensorType == TYPE_PHOTO){
        m_upUrl = QUrl(baseUrl + "photos");
        m_downUrl = QUrl(baseUrl + "photo/content");
    }
}
void YeelinkObject::post(QByteArray array)
{
    QNetworkRequest request(m_upUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request.setRawHeader(QByteArray("U-ApiKey"),m_apiKey.toLatin1());
    request.setRawHeader(QByteArray("Host"),QByteArray("api.yeelink.net"));

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    manager->post(request,array);
    connect (manager, &QNetworkAccessManager::finished, [=](QNetworkReply *reply) {
        emit sig_requestResult(reply->readAll());
    });
}
void YeelinkObject::get()
{
    QNetworkRequest request(m_downUrl);
    request.setRawHeader(QByteArray("U-ApiKey"),m_apiKey.toLatin1());
    request.setRawHeader(QByteArray("Host"),QByteArray("api.yeelink.net"));

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    connect (manager, &QNetworkAccessManager::finished, [=](QNetworkReply *reply) {
        emit sig_yellikResponse(reply->readAll());
    });
    manager->get(request);
}

YeelinkValueTypeSensor::YeelinkValueTypeSensor(QObject *parent) :
    YeelinkObject(parent)
{
    m_sensorType = SensorType::TYPE_VALUE;
}

void YeelinkValueTypeSensor::setValTypeData(double value)
{
    QJsonObject json;
    json.insert("value",value);
    QJsonDocument document;
    document.setObject(json);
    QByteArray array = document.toJson(QJsonDocument::Compact);
    this->post(array);
}
void YeelinkValueTypeSensor::getValStatus()
{
    this->get();
    connect(this,&YeelinkObject::sig_yellikResponse,[=](QByteArray data){
        QJsonParseError jParseError;
        QJsonDocument json = QJsonDocument::fromJson(data,&jParseError);
        if(jParseError.error == QJsonParseError::NoError){
            QJsonObject bodyObj = json.object();
            emit sig_valData(bodyObj["value"].toDouble());
        }
    });
}

YeelinkBoolTypeSensor::YeelinkBoolTypeSensor(QObject *parent) :
    YeelinkObject(parent)
{
    m_sensorType = SensorType::TYPE_BOOL;
}
void YeelinkBoolTypeSensor::setBoolTypeData(bool status)
{
    QJsonObject json;
    json.insert("value",status?1:0);
    QJsonDocument document;
    document.setObject(json);
    QByteArray array = document.toJson(QJsonDocument::Compact);

    this->post(array);
}
void YeelinkBoolTypeSensor::getBoolStatus()
{
    this->get();
    connect(this,&YeelinkObject::sig_yellikResponse,[=](QByteArray data){
        QJsonParseError jParseError;
        QJsonDocument json = QJsonDocument::fromJson(data,&jParseError);
        if(jParseError.error == QJsonParseError::NoError){
            QJsonObject bodyObj = json.object();
            emit sig_boolData(bodyObj["value"].toInt());
        }
    });
}


YeelinkPhotoTypeSensor::YeelinkPhotoTypeSensor(QObject *parent) :
    YeelinkObject(parent)
{
    m_sensorType = SensorType::TYPE_PHOTO;
}
void YeelinkPhotoTypeSensor::setPhotoTypeData(QByteArray data)
{
    this->post(data);
}
void YeelinkPhotoTypeSensor::getPhotoData()
{
    this->get();
    connect(this,&YeelinkObject::sig_yellikResponse,[=](QByteArray data){
        emit sig_photoData(data);
    });
}

YeelinkGPSTypeSensor::YeelinkGPSTypeSensor(QObject *parent) :
    YeelinkObject(parent)
{
    m_sensorType = SensorType::TYPE_GPS;
}
void YeelinkGPSTypeSensor::setGPSTypeData(double lat,double lng,double speed)
{
    QJsonObject gpsJson;
    gpsJson.insert("lat",lat);
    gpsJson.insert("lng",lng);
    gpsJson.insert("speed",speed);

    QJsonObject json;
    json.insert("value",gpsJson);
    QJsonDocument document;
    document.setObject(json);
    QByteArray array = document.toJson(QJsonDocument::Compact);
    this->post(array);
}
void YeelinkGPSTypeSensor::getGPSData()
{
    this->get();
    connect(this,&YeelinkObject::sig_yellikResponse,[=](QByteArray data){
        QJsonParseError jParseError;
        QJsonDocument json = QJsonDocument::fromJson(data,&jParseError);
        if(jParseError.error == QJsonParseError::NoError){
            QJsonObject bodyObj = json.object();
            emit sig_GPSData(bodyObj["value"].toObject()["lat"].toDouble()
                    ,bodyObj["value"].toObject()["lng"].toDouble()
                    ,bodyObj["value"].toObject()["speed"].toDouble());
        }
    });
}

YeelinkGenericTypeSensor::YeelinkGenericTypeSensor(QObject *parent) :
    YeelinkObject(parent)
{
    m_sensorType = SensorType::TYPE_GENERIC;
}
void YeelinkGenericTypeSensor::setGenericTypeData(QByteArray key,QByteArray data)
{
    QJsonObject jsonGeneric = QJsonDocument::fromJson(data).object();
    QJsonObject json;
    json.insert("value",jsonGeneric);
    json.insert("key",key.data());
    QJsonDocument document;
    document.setObject(json);
    QByteArray array = document.toJson(QJsonDocument::Compact);
    this->post(array);
}
void YeelinkGenericTypeSensor::getGenericData()
{
    this->get();
    connect(this,&YeelinkObject::sig_yellikResponse,[=](QByteArray data){
        QJsonParseError jParseError;
        QJsonDocument json = QJsonDocument::fromJson(data,&jParseError);
        if(jParseError.error == QJsonParseError::NoError){
            QJsonObject bodyObj = json.object();
            QString genericJson = QJsonDocument(bodyObj["value"].toObject()).toJson();
            emit sig_GenericData(genericJson);
        }
    });
}
