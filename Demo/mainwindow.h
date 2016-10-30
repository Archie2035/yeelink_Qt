#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "yeelinkapi_v1.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString appKey;
    QString deviceId;
    QString localPicPath;

    void loadConfig();
    YeelinkValueTypeSensor *yeelinkValueSensor;
    YeelinkBoolTypeSensor *yeelinkBoolSensor;
    YeelinkGPSTypeSensor *yeelinkGPSSensor;
    YeelinkGenericTypeSensor *yeelinkGenericSensor;
    YeelinkPhotoTypeSensor *yeelinkPhotoSensor;

private slots:

    void slt_showMessage(QByteArray reply);

    void on_pushButtonValueUpload_clicked();
    void on_pushButtonBoolOnUpload_clicked();
    void on_pushButtonBoolOffUpload_clicked();
    void on_pushButtonGPSUpload_clicked();
    void on_pushButtonGenericUpload_clicked();
    void on_pushButtonOpenLocalPic_clicked();
    void on_pushButtonPhotoUpload_clicked();

    void on_pushButtonValueDownload_clicked();

    void on_pushButtonBoolDownload_clicked();

    void on_pushButtonGPSDownload_clicked();

    void on_pushButtonGenericDownload_clicked();

    void on_pushButtonPhotoDownload_clicked();

    void on_pushButtonAboutMeGithub_clicked();

    void on_pushButtonAboutMeCSDN_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
