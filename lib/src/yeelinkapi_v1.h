#ifndef HTTPAPI_H
#define HTTPAPI_H

#include <QObject>

#include <QHttpPart>
#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>

enum SensorType{
    TYPE_VALUE = 0,     //数值型传感器
    TYPE_BOOL = 1,      //开关型传感器
    TYPE_GPS = 2,       //GPS型传感器
    TYPE_GENERIC = 3,   //泛型传感器
    TYPE_PHOTO = 4      //图片行传感器
};
class YeelinkObject : public QObject
{
    Q_OBJECT
public:
    YeelinkObject(QObject *parent = 0);
    void init(QString deviceid,QString sensorid,QString apikey);
protected:
    SensorType m_sensorType;
    QUrl m_upUrl;
    QUrl m_downUrl;
    QString m_sensorId;
    QString m_deviceId;
    QString m_apiKey;
    void post(QByteArray data);
    void get(void);
signals:
    void sig_requestResult(QByteArray data);
    void sig_yellikResponse(QByteArray data);
};

class YeelinkValueTypeSensor :public YeelinkObject
{
    Q_OBJECT
public:
    YeelinkValueTypeSensor(QObject *parent = 0);

    void setValTypeData(double value);
    void getValStatus(void);
private:

private slots:
signals:
    void sig_valData(double);
};

class YeelinkBoolTypeSensor :public YeelinkObject
{
    Q_OBJECT
public:
    YeelinkBoolTypeSensor(QObject *parent = 0);

    void setBoolTypeData(bool status);
    void getBoolStatus(void);
private:


private slots:
signals:
    void sig_boolData(bool);
};

class YeelinkPhotoTypeSensor :public YeelinkObject
{
    Q_OBJECT
public:
    YeelinkPhotoTypeSensor(QObject *parent = 0);

    void setPhotoTypeData(QByteArray data);
    void getPhotoData(void);
private:


private slots:
signals:
    void sig_photoData(QByteArray);
};

class YeelinkGPSTypeSensor :public YeelinkObject
{
    Q_OBJECT
public:
    YeelinkGPSTypeSensor(QObject *parent = 0);

    void setGPSTypeData(double lat,double lng,double speed);
    void getGPSData(void);
private:


private slots:
signals:
    void sig_GPSData(double lat,double lng,double speed);
};

class YeelinkGenericTypeSensor :public YeelinkObject
{
    Q_OBJECT
public:
    YeelinkGenericTypeSensor(QObject *parent = 0);

    void setGenericTypeData(QByteArray key,QByteArray data);
    void getGenericData(void);
private:


private slots:
signals:
    void sig_GenericData(QString data);
};

#endif // HTTPAPI_H
