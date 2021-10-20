#include "zmqserver.h"
#include "ui_zmqserver.h"
#include "zmq.hpp"

zmqServer::zmqServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::zmqServer)
{
    ui->setupUi(this);
    //поиск своего айпи адреса
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if(address.protocol()== QAbstractSocket::IPv4Protocol && address != localhost){
            //вывод своего айпи
            ui->lblMyIp->setText(address.toString());}}
            ui->btnClose->setEnabled(false);
    //перемещаем класс в тред и запускаем тред
    ZMQserver.moveToThread(&thread);
    thread.start();
    //connect.............................................................
    connect(ui->btnConnect,SIGNAL(clicked()),&ZMQserver,SLOT(Work()));
    connect(ui->btnClose,SIGNAL(clicked()),&ZMQserver,SLOT(Finish()));
    connect(&ZMQserver,SIGNAL(workFinished()),&thread,SLOT(quit()));
    //
//    connect(ui->btnConnect,SIGNAL(clicked()),SLOT(slotStart()));
//    connect(this,SIGNAL(workStart()),&ZMQserver,SLOT(Work()));
//    connect(ui->btnClose,SIGNAL(clicked()),SLOT(slotFinish()));
//    connect(this,SIGNAL(FinishWork()),&ZMQserver,SLOT(Finish()));
    //
    connect(&ZMQserver,SIGNAL(doWork()),SLOT(slotStart()));
    connect(&ZMQserver,SIGNAL(isFinish()),SLOT(slotFinish()));
    connect(&ZMQserver,SIGNAL(MsgGet(QString)),SLOT(slotReleaseMsg(QString)));
    connect(&ZMQserver,SIGNAL(ImgGet(QByteArray)),SLOT(slotReleaseImg(QByteArray)));
    connect(&ZMQserver,SIGNAL(ConnectDisconnectGet(QString)),
            SLOT(slotConnectDisconnectClient(QString)));
    connect(&ZMQserver,SIGNAL(RestartWork()),&ZMQserver,SLOT(Work()));
    //connect.............................................................
}

void zmqServer::slotStart(){
    qDebug()<<"workstart in main";
    QLabel *lblStart = new QLabel;
    lblStart->setText(QTime::currentTime().toString() + " Server ready to catch...");
    ui->vrtlyt->addWidget(lblStart);
    ui->btnConnect->setEnabled(false);
    ui->btnClose->setEnabled(true);
    int one;
    int two;
    int free;
    zmq::version(&one,&two,&free);
        qDebug()<<"version: " <<one<<two<<free;
}

void zmqServer::slotReleaseMsg(QString newMsg){
    qDebug()<<"slotRelease "<<newMsg;
    QLabel *lblMsg = new QLabel;
    lblMsg->setText(QTime::currentTime().toString() + " " + newMsg);
    ui->vrtlyt->addWidget(lblMsg);
}

void zmqServer::slotReleaseImg(QByteArray newImg){
    QPixmap p/*(newImg)*/;
    p.loadFromData(newImg,"PNG");
    QLabel *lbl = new QLabel;
    int imgWidth = 200;
    int imgHeight = 200;
    QSize sizeImg(imgWidth,imgHeight);
    lbl->setPixmap(p.scaled(sizeImg,Qt::IgnoreAspectRatio,Qt::FastTransformation));
    ui->vrtlyt->addWidget(lbl);
}

void zmqServer::slotConnectDisconnectClient(QString ConDisMsg){
    QLabel *lbl = new QLabel;
    qDebug()<<"length condismsg"<<ConDisMsg.length();
    lbl->setText(QTime::currentTime().toString() + " " + ConDisMsg);
    ui->vrtlyt->addWidget(lbl);
    QString str1,str2;
    str1 = ConDisMsg.section(" ",2,-1);
    str2 = ConDisMsg.section(" ",0,1);
    qDebug()<<"qstring: "<<ConDisMsg;
    qDebug()<<"qstring1: "<<str1;
    qDebug()<<"qstring1: "<<str1.length();
    qDebug()<<"qstring2: "<<str2;
    qDebug()<<"qstring2: "<<str2.length();
//        QTreeWidgetItem *item = new QTreeWidgetItem();
    qDebug()<< "sddfsdfs " <<str2 + " " + str1;
    QString var = str2;

        if(str2 + " " + str1 == var + " " + "connected to server...") {
            list.append(str2);
            qDebug()<<"list append: "<< list.toVector();
            qDebug()<<"list append: "<< list.at(0);
        } else {
        for(int i = 0;i <= list.length();i++){
           if(var + " disconnected to server..." == ConDisMsg){
               for(int i = 0; i<= list.length(); i++){
                    if(list.at(i) == str2) {
                       //if(list.at(i) == str2){
                        //list.takeAt(i);
                        qDebug()<<" list remove: "<<list.at(i);
                        qDebug()<<" list remove: "<<list.length();
                       } else {
                           qDebug()<<"problem in delete pc: "<<list.at(i);
                            qDebug()<<"problem in delete pc: "<<list.length();
                        }
                    //}
             }
                   qDebug()<<"list remove: "<< list.toVector();
        }
      }
 }
}

void zmqServer::slotFinish(){
    QLabel *lblFinish = new QLabel;
    lblFinish->setText(QTime::currentTime().toString() + " Server close connection...");
    ui->vrtlyt->addWidget(lblFinish);
    ui->btnConnect->setEnabled(true);
    ui->btnClose->setEnabled(false);
    ui->treeConClient->clear();
    //emit FinishWork();
}

zmqServer::~zmqServer()
{
    delete ui;
}

//qDebug()<<"slotReleaseImg "<<newImg;
//    QLabel *lblImg = new QLabel;
//    lblImg->setText(QTime::currentTime().toString() + " получил картинку...");
//    ui->vrtlyt->addWidget(lblImg);
//    QFile file/*("~/Desktop/file.png")*/;
//    file.open(QIODevice::WriteOnly);
//        qDebug()<<"file before writing: "<<file.size();
//    file.write(newImg);
//        qDebug()<<"file after writing: "<<file.size();
//    //file = QFileDialog::getSaveFileName(this,tr("Save file"),"/opt/newfile",tr("Images (*.png *.xpm *.jpg)"));
//    file.close();


//    if(str1 == "connected to server..."){
//        item->setText(0,str2);
//        ui->treeConClient->addTopLevelItem(item);
//        list.append(ConDisMsg);
//        qDebug()<<"elements:" <<list.toVector();
//    } else   if(str1 == "disconnected to server..."){

//    }



//    if(str2 + " " + str1 == var + " " + "connected to server...") {
//        list.append(str2);
//        qDebug()<<"list append: "<< list.toVector();
//        qDebug()<<"list append: "<< list.at(0);
//    } else {
//        for(int i = 0;i <= list.length();i++){
//            if(var + " disconnected to server..." == ConDisMsg){

//           for(int i = 0; i<= list.length(); i++){
//                if(list.at(i) == str2) {
//                   if(list.at(i) == str2){
//                    //list.takeAt(i);
//                    qDebug()<<" list remove: "<<list.at(i);
//                    qDebug()<<" list remove: "<<list.length();
//                   } else {
//                       qDebug()<<"problem in delete pc: "<<list.at(i);
//                        qDebug()<<"problem in delete pc: "<<list.length();
//                    }
//                }
//            }
//               qDebug()<<"list remove: "<< list.toVector();
//            }
//        }
//    }
