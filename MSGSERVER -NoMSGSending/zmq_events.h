#ifndef MY_EVENTS_H
#define MY_EVENTS_H

#include <QDataStream>

//#define I_QDATASTREAMVERSION QDataStream::Qt_5_6    //TODO: Probably use newer version? Probably transmit version with packet?
//см. deserializeFromZMQMessage
#define I_QDATASTREAMVERSION QDataStream::Qt_4_8    //ГИС Оператор 11 поддерживает плагины только на 4 Qt.


#endif // MY_EVENTS_H
