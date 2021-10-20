#include "client.h"
#include "ui_client.h"
#include "zmq.h"
#include "zmq.hpp"
//
#include <QBitmap>

Client::Client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    connect(ui->btnPush,SIGNAL(clicked()),SLOT(Connect()));
    connect(ui->btnClose,SIGNAL(clicked()),SLOT(Destroy()));
    connect(ui->btnSend,SIGNAL(clicked()),SLOT(Send()));
    connect(ui->btnSendIMG,SIGNAL(clicked()),SLOT(SendImage()));
    connect(this,SIGNAL(Error()),SLOT(Destroy()));

//поиск своего айпи
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if(address.protocol()== QAbstractSocket::IPv4Protocol && address != localhost){
            ui->lblMyIP->setText(address.toString());
        }
    }
    ipAd = "tcp://127.0.0.1";
    ui->lnIp->setText(ipAd);
    myPort = ":2323";
    ui->lnName->setText(myName);
    ui->lnPort->setText(myPort);
    ui->btnSend->setEnabled(false);
    ui->btnClose->setEnabled(false);
    ui->btnSendIMG->setEnabled(false);
    ui->btnWho->setEnabled(false);
}

void Client::Connect(){
    context = zmq_ctx_new();
    socket = zmq_socket(context, ZMQ_PUSH);
    QLabel *lbl = new QLabel;
    lbl->setText(QTime::currentTime().toString()
                 + " Connect... ");
    ui->vrtLayout->addWidget(lbl,0,Qt::AlignLeft );
    int rc;
    rc = zmq_connect(socket,ui->lnIp->text().toUtf8() + ui->lnPort->text().toUtf8());
    if(rc == -1){
        QLabel *lblErr = new QLabel;
        lblErr->setText(QTime::currentTime().toString()
                     + " Error on connect.. ");
        emit Error();
    } else {
    ui->btnSend->setEnabled(true);
    ui->btnSendIMG->setEnabled(true);
    ui->btnPush->setEnabled(false);
    ui->btnClose->setEnabled(true);
    ui->btnWho->setEnabled(true);
    }
    zmq_send(socket,ui->lnName->text().toUtf8() + " connected to server...",60,0);
}

void Client::Destroy(){
    QLabel *lbl = new QLabel;
    lbl->setText(QTime::currentTime().toString()
                 + " Destroy connect...");
    ui->vrtLayout->addWidget(lbl,0,Qt::AlignLeft );
    zmq_send(socket,ui->lnName->text().toUtf8() + " disconnected to server...",60,0);
    zmq_close(socket);
    zmq_ctx_destroy(context);
    ui->btnSend->setEnabled(false);
    ui->btnSendIMG->setEnabled(false);
    ui->btnClose->setEnabled(false);
    ui->btnPush->setEnabled(true);
    ui->btnWho->setEnabled(false);
}

void Client::Send(){
    zmq_msg_t msg;
    int rc = zmq_msg_init_size(&msg, 256);
    if(!rc == 0) {
        QLabel *lblErr = new QLabel;
        lblErr->setText(QTime::currentTime().toString()
                     + " Error: problem in initialization message...");
                emit Error();
    }
    if(ui->lnSend->text().length() != 0){
    rc = zmq_send(socket,ui->lnName->text().toUtf8() + " sending: " + ui->lnSend->text().toUtf8(),256,0);
    if(!rc == 256) {
        QLabel *lblErr = new QLabel;
        lblErr->setText(QTime::currentTime().toString()
                     + " Error: problem on sending message...");
                emit Error();
    }}
//отправка картинок
    if (CompArr.length() > 0){
     zmq_send(socket,ui->lnName->text().toUtf8() + " sending image: ",256,0);
     zmq_send(socket,CompArr,CompArr.length(),0);
     CompArr.clear();
     qDebug()<<"comp arr: "<< CompArr.length();
    }
//если надо отправить запрос серверу, только с типом запрос-ответ(ZMQ_REP,ZMQ_REQ)
    //zmq_recv(socket,buff,20,0);
    QLabel *lbl = new QLabel;
    lbl->setText(QTime::currentTime().toString()
                 + " Sending: " + ui->lnSend->text().toUtf8());
    ui->vrtLayout->addWidget(lbl,0,Qt::AlignLeft );
    rc = zmq_msg_close(&msg);
    if(!rc == 0) {
        QLabel *lblErr = new QLabel;
        lblErr->setText(QTime::currentTime().toString()
                     + " Error: problem with close message...");
                emit Error();
    }
    ui->lnSend->setText("");
}

void Client::SendImage(){
    ui->btnSend->setEnabled(false);
    QString fl;
    fl = QFileDialog::getOpenFileName(this,tr("выберите изображение"),"/opt",
                              tr("файлы изображений (* .png * .jpg * .bmp)"));
    qDebug()<<fl;
    QPixmap img(fl);
    QFile f(fl);
    f.open(QIODevice::ReadOnly);
    QByteArray a = f.readAll();
    qDebug()<<"qbytearray file: "<<a;
    qDebug()<<"qbytearray file length: "<<a.length();
    qDebug()<<"qbytearray file data: "<<a.data();
    qDebug()<<img;
    qDebug()<<img.size();
    CompArr = a;
    qDebug()<<"acompArr: "<<CompArr.length();
    if(CompArr.length() > 0){
    qDebug()<<"img ready to send";
    int imgWidth = 200;
    int imgHeight = 200;
    QSize sizeImg(imgWidth,imgHeight);
    QLabel *lbl = new QLabel;
    qDebug()<<"size : "<<img;
    lbl->setPixmap(img.scaled(sizeImg,Qt::IgnoreAspectRatio,Qt::FastTransformation));
    ui->vrtLayout->addWidget(lbl);
    QLabel *lbl2 = new QLabel;
    lbl2->setText(QTime::currentTime().toString() + " img ready to send...");
    ui->vrtLayout->addWidget(lbl2);
    ui->btnSend->setEnabled(true);
    } else {
    ui->btnSend->setEnabled(true);
    QLabel *lblErr = new QLabel;
            lblErr->setText(QTime::currentTime().toString()
                         + " Error...");
    }
}

Client::~Client()
{
    delete ui;
}
