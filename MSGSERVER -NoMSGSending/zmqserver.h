#pragma once
#include <QNetworkInterface>
#include <QHostAddress>
#include <QMainWindow>
#include <QThread>
#include <QFile>
#include <QSaveFile>
#include <QFileDialog>
#include <QLabel>
#include <QPixmap>
#include <QLinkedList>
#include <QTreeWidgetItem>
#include <QVector>
#include "inthread.h"

namespace Ui {
class zmqServer;
class inThread;
}

class zmqServer : public QMainWindow
{
    Q_OBJECT
public:
    bool isRunning();
    explicit zmqServer(QWidget *parent = 0);
    Ui::zmqServer *ui;
    QList<QString> list;
    QVector<QString> vec;
    ~zmqServer();
public slots:
    void slotStart();
    void slotFinish();
    void slotReleaseMsg(QString newMsg);
    void slotReleaseImg(QByteArray newImg);
    void slotConnectDisconnectClient(QString ConDisMsg);
signals:
    void FinishWork();
    void workStart();
private:
    QThread thread;
    inThread ZMQserver;
};


