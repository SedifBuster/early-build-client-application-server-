#pragma once

#include <QMainWindow>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QTime>
#include <QFile>
#include <QFileDialog>
#include <QPixmap>
#include <QBuffer>
#include <QByteArray>
#include <QVBoxLayout>


namespace Ui {
class Client;
}

class Client : public QMainWindow
{
    Q_OBJECT

public:
    QString ipAd;
    QString myPort;
    QString myName = "Компьютер 1";
    QFile fileImg;
    QVBoxLayout bxl2;
    QByteArray CompArr = nullptr;
    explicit Client(QWidget *parent = 0);
    ~Client();
signals:
    void Error();
public slots:
    void Connect();
    void Destroy();
    void Send();
    void SendImage();
private:
    void* context;
    void* socket;


    Ui::Client *ui;
};


