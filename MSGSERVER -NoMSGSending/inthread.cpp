#include "inthread.h"
#include "zmqserver.h"
#include "compat.h"
#include <zmq_events.h>
#include <zmq_helper_functions.h>
#include <memory>

inThread::inThread(QObject *parent) : QObject(parent),context_(1)
{ socket_ = std::make_unique<zmq::socket_t>(context_, ZMQ_PULL);
  socket_->bind("tcp://127.0.0.1:2323");
}

void inThread::Work(){
    emit doWork();
    zmq::message_t msg;
    int nbytes = socket_->recv(&msg,0);
    if (nbytes >= 0){
        qDebug()<<"увидел байты, получаю...";
        qDebug()<<"кол-во байтов в сообщении zmq: "<< msg.size();
    QByteArray buffer(static_cast<const char*>(msg.data()), msg.size());
        qDebug()<<"кол-во байтов в сообщении: "<< buffer.length();
        qDebug()<<"состав сообщения: "<< QString(buffer);
    if(buffer.size() == 256){
//выводим текстовое сообщение в ui
    emit MsgGet(buffer);
    }
    else if (buffer.size() > 256) {
//выводим картинку в ui
    emit ImgGet(buffer);
       }
    else if (buffer.size() == 60){
//коннект дисконнект отображение
    emit ConnectDisconnectGet(buffer);
    }
    }
//если надо отправить ответ клиенту, только с типом запрос-ответ(ZMQ_REP,ZMQ_REQ)
    //socket_->send(&msg,0);
//уничтожаем сообщение и вызываем сигнал рестарта слота Work
    msg.~message_t();
    emit RestartWork();
}

void inThread::Finish(){
    emit isFinish();
    qDebug()<<"workfinished";
    context_.close();
}

inThread::~inThread(){

    }

//отловить айпи::zmq_monitor?
    //документация?
    //сделать сервер и клиент без ответного сообщения::YES
    //скорость сетевого подключения:: определяется протоколом?
//выход из цикла при закрытии программы
//бинд на адресс
//оживить кнопки
//finish?переместить в zmqserver.cpp

//первая нерабочая попытка zmq
//            zmq_msg_t part;
//            int rec;
//            rec = zmq_msg_init(&part);
//            qDebug()<<"код ошибки: "<<rec;
//            assert (rec == 0);
//            rec = zmq_recv(socket,buff,256,0);
//            arr = buff;
//            QByteArray::fromBase64(arr);
//            qDebug()<<"получил сообщение"<<arr;
//            emit MsgGet(arr);
//            qDebug()<<"geting message";
//            assert (rec != 1);
//            qDebug()<<"код ошибки: "<<rec; //
//            zmq_msg_close(&part);
//            qDebug()<<"slot end running";
//               stop_ = true;
//              stop_ = false;

//вторая версия с хелпером михаила, сама жизнеспособная, получает сообщения, затирает их,
// проблема с копированием с zmq сообщений в qbytearray частично решена memcpy,
// но после получения первого сообщения падает с ошибкой памяти
//.................main///////////////
//socket_ = std::make_unique<zmq::socket_t>(context_, ZMQ_REP);
 //bindZMQSocket(socket_, 2323, "SERVER");
//...................................
//    while(!stop_) {
//            zmq::poll(items, IF::sizeOfArray(items), 1);
//            if (items[0].revents & ZMQ_POLLIN) {
//                zmq::message_t part;
//                socket_->recv(&part);
//    //копируем значение из полученного сообщения
//                memcpy(&buff,part.data(),20);
//                arr = &buff;
//                QByteArray::fromBase64(arr);
//    qDebug()<<"кол-во байтов в сообщении"<< part.size();
//    qDebug()<<"получил сообщение в байтах"<< items;
//    qDebug()<<"получил сообщение после memcpy"<< arr;
//    //выводим сообщение в ui
//                emit MsgGet(arr);
//                part.rebuild(items,256);
//                   }
//                  }
//    //скорость сетевого подключения
//    is_running_ = false;

//третья версия с урезанным zmq без стд не убирает полученные сообщения в последней строке,
//делая бесконечное получение сообщений.
//zmq_msg_t msg;
//int res;
//res = zmq_msg_init(&msg);
//int nbytes = zmq_msg_recv(&msg,socket,0);
//if (nbytes >= 0){
//    qDebug()<<"увидел байты, получаю...";
//    QByteArray buffer(static_cast<const char*>(msg));
//    QByteArray buffer("asdasdasd3234234");
//    QDataStream data_stream(&buffer, QIODevice::ReadOnly);
//    data_stream.setVersion(I_QDATASTREAMVERSION);
//}

//qDebug()<<"полученные сообщения: "<< &msg;

//выводим сообщение в ui
// emit MsgGet(arr);

//zmq_msg_close(&msg);
