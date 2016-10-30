#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QBuffer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUuid>
#include <QFileDialog>
#include <QSettings>
#include <QDesktopServices>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings *configIniRead = new QSettings("yeelink.ini", QSettings::IniFormat);
    QString appkey = configIniRead->value("/app/apikey").toString();
    QString deviceid = configIniRead->value("/app/deviceid").toString();

    QString valuesensorid = configIniRead->value("/sensor/valuesensor").toString();
    QString boolsensorid = configIniRead->value("/sensor/boolsensor").toString();
    QString gpssensorid = configIniRead->value("/sensor/gpssensor").toString();
    QString genericsensorid = configIniRead->value("/sensor/genericsensor").toString();
    QString picsensorid = configIniRead->value("/sensor/picsensor").toString();

    ui->lineEditAPIKey->setText(appkey);
    ui->lineEditDeviceId->setText(deviceid);
    ui->lineEditValueSensorId->setText(valuesensorid);
    ui->lineEditBoolSensorId->setText(boolsensorid);
    ui->lineEditGPSSensorId->setText(gpssensorid);
    ui->lineEditGenericSensorId->setText(genericsensorid);
    ui->lineEditPicSensorId->setText(picsensorid);

    yeelinkValueSensor = new YeelinkValueTypeSensor();
    yeelinkBoolSensor = new YeelinkBoolTypeSensor();
    yeelinkGPSSensor = new YeelinkGPSTypeSensor();
    yeelinkGenericSensor = new YeelinkGenericTypeSensor();
    yeelinkPhotoSensor = new YeelinkPhotoTypeSensor();

    connect(yeelinkValueSensor,SIGNAL(sig_requestResult(QByteArray)),this,SLOT(slt_showMessage(QByteArray)));
    connect(yeelinkBoolSensor,SIGNAL(sig_requestResult(QByteArray)),this,SLOT(slt_showMessage(QByteArray)));
    connect(yeelinkGPSSensor,SIGNAL(sig_requestResult(QByteArray)),this,SLOT(slt_showMessage(QByteArray)));
    connect(yeelinkGenericSensor,SIGNAL(sig_requestResult(QByteArray)),this,SLOT(slt_showMessage(QByteArray)));
    connect(yeelinkPhotoSensor,SIGNAL(sig_requestResult(QByteArray)),this,SLOT(slt_showMessage(QByteArray)));

    connect(yeelinkValueSensor,&YeelinkValueTypeSensor::sig_valData,[=](double data){
        ui->lineEditCurrentVal->setText(QString("%1").arg(data));
    });

    connect(yeelinkBoolSensor,&YeelinkBoolTypeSensor::sig_boolData,[=](bool open){
        ui->lineEditCurrentBool->setText(QString("%1").arg(open?"on":"off"));
    });

    connect(yeelinkGPSSensor,&YeelinkGPSTypeSensor::sig_GPSData,[=](double lat,double lng,double speed){
        ui->lineEditCurrentLat->setText(QString("%1").arg(lat));
        ui->lineEditCurrentLng->setText(QString("%1").arg(lng));
        ui->lineEditCurrentSpeed->setText(QString("%1").arg(speed));
    });

    connect(yeelinkGenericSensor,&YeelinkGenericTypeSensor::sig_GenericData,[=](QString data){
        ui->textEditCurrentGenericData->setText(data);
    });

    connect(yeelinkPhotoSensor,&YeelinkPhotoTypeSensor::sig_photoData,[=](QByteArray data){
        QImage img;
        bool beLoad = img.loadFromData(data);
        img = img.scaled(ui->labelCurrentPic->size());
        if(!beLoad)
            ui->statusBar->showMessage("can't get the pic!",5000);
        ui->labelCurrentPic->setPixmap(QPixmap::fromImage(img));
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::slt_showMessage(QByteArray reply)
{
    QString message;
    message = reply;
    if(reply.isEmpty())
        message = "Success!";
    ui->statusBar->showMessage(message,5000);
}
void MainWindow::loadConfig()
{
    QSettings *configIniWrite = new QSettings("yeelink.ini", QSettings::IniFormat);


    appKey = ui->lineEditAPIKey->text();
    deviceId = ui->lineEditDeviceId->text();
    QString valueSensorId = ui->lineEditValueSensorId->text();
    QString boolSensorId = ui->lineEditBoolSensorId->text();
    QString GPSSensorId = ui->lineEditGPSSensorId->text();
    QString genericSensorId = ui->lineEditGenericSensorId->text();
    QString picSensorId = ui->lineEditPicSensorId->text();

    yeelinkValueSensor->init(deviceId,valueSensorId,appKey);
    yeelinkBoolSensor->init(deviceId,boolSensorId,appKey);
    yeelinkGPSSensor->init(deviceId,GPSSensorId,appKey);
    yeelinkGenericSensor->init(deviceId,genericSensorId,appKey);
    yeelinkPhotoSensor->init(deviceId,picSensorId,appKey);

    configIniWrite->setValue("/app/apikey",appKey);
    configIniWrite->setValue("/app/deviceid",deviceId);
    configIniWrite->setValue("/sensor/valuesensor",valueSensorId);
    configIniWrite->setValue("/sensor/boolsensor",boolSensorId);
    configIniWrite->setValue("/sensor/gpssensor",GPSSensorId);
    configIniWrite->setValue("/sensor/genericsensor",genericSensorId);
    configIniWrite->setValue("/sensor/picsensor",picSensorId);
}

void MainWindow::on_pushButtonValueUpload_clicked()
{
    loadConfig();
    yeelinkValueSensor->setValTypeData(ui->lineEditValueSensorData->text().toDouble());
}

void MainWindow::on_pushButtonBoolOnUpload_clicked()
{
    loadConfig();
    yeelinkBoolSensor->setBoolTypeData(true);
}

void MainWindow::on_pushButtonBoolOffUpload_clicked()
{
    loadConfig();
    yeelinkBoolSensor->setBoolTypeData(false);
}

void MainWindow::on_pushButtonGPSUpload_clicked()
{
    loadConfig();
    yeelinkGPSSensor->setGPSTypeData(ui->lineEditGPSSensorLatData->text().toDouble()
                                     ,ui->lineEditGPSSensorLngData->text().toDouble()
                                     ,ui->lineEditGPSSensorSpeedData->text().toDouble());
}

void MainWindow::on_pushButtonGenericUpload_clicked()
{
    loadConfig();
    QUuid uuid = QUuid::createUuid();
    QByteArray key = uuid.toString().toLatin1();
    QByteArray array = ui->textEditGenericJsonData->toPlainText().toLatin1();
    yeelinkGenericSensor->setGenericTypeData(key,array);
}

void MainWindow::on_pushButtonOpenLocalPic_clicked()
{
    localPicPath = QFileDialog::getOpenFileName(this,"select pic",QDir::currentPath(),"Images (*.jpg)");
    ui->lineEditLocalPicPath->setText(localPicPath);
}

void MainWindow::on_pushButtonPhotoUpload_clicked()
{
    loadConfig();
    char* m_buf;
    QFile file(localPicPath);
    file.open(QIODevice::ReadOnly);
    qint64 file_len = file.size();
    QDataStream in(&file);
    m_buf = new char[file_len];
    in.readRawData( m_buf, file_len);
    file.close();

    QByteArray arr = QByteArray(m_buf, file_len);

    yeelinkPhotoSensor->setPhotoTypeData(arr);
}

void MainWindow::on_pushButtonValueDownload_clicked()
{
    loadConfig();
    yeelinkValueSensor->getValStatus();
}

void MainWindow::on_pushButtonBoolDownload_clicked()
{
    loadConfig();
    yeelinkBoolSensor->getBoolStatus();
}

void MainWindow::on_pushButtonGPSDownload_clicked()
{
    loadConfig();
    yeelinkGPSSensor->getGPSData();
}

void MainWindow::on_pushButtonGenericDownload_clicked()
{
    loadConfig();
    yeelinkGenericSensor->getGenericData();
}

void MainWindow::on_pushButtonPhotoDownload_clicked()
{
    loadConfig();
    yeelinkPhotoSensor->getPhotoData();
}

void MainWindow::on_pushButtonAboutMeGithub_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/tianzhihen/yeelink_Qt"));
}

void MainWindow::on_pushButtonAboutMeCSDN_clicked()
{
    QDesktopServices::openUrl(QUrl("http://blog.csdn.net/tianzhihen_wq/article/details/52974107"));
}
