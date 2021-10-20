#ifndef INTHREAD_H
#define INTHREAD_H

#include "zmq.hpp"
#include <QObject>
#include <QDebug>
#include <QByteArray>
#include <QTimer>
#include <QTime>
#include <QDataStream>
//#include <zmq_events.h>
#include <zmq_helper_functions.h>


class inThread : public QObject
{
    Q_OBJECT
public:
    explicit inThread(QObject *parent = nullptr);
    ~inThread();
signals:
    void doWork();
    void isFinish();
    void MsgGet(QString msg);
    void ImgGet(QByteArray newImg);
    void ConnectDisconnectGet(QString ConDismsg);
    void RestartWork();
public slots:
    void Work();
    void Finish();
private:
    char buff;
    std::unique_ptr<zmq::socket_t> socket_;
    zmq::context_t context_;
    QByteArray arr;
};

#endif // INTHREAD_H
