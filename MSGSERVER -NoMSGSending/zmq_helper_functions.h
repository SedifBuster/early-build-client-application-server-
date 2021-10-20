#ifndef ZMQHELPERFUNCTIONS_H
#define ZMQHELPERFUNCTIONS_H

#include <ifunctions.h>
#include <soord_constants.h>
#include <zmq_events.h>

#include <zmq.hpp>

#include <QByteArray>
#include <QDataStream>
#include <QFileInfo>
#include <QStringBuilder>
#include <QStringList>

#include <memory>
#include <string>
#include <typeinfo>
//TODO: wrap functions to namespace?

/**
 * @brief The BasePorts enum contains base ports numbers
 * NOTE: in/out direction is relevant to Dispatcher/Server/Logger.
 * It is reversed from the point of other peers.
 */
struct BasePorts {
enum Ports:int {
    logs_in         = 12000,
    logs_hb         = 12100,    //!< heartbeat
    bus_message_in  = 35221,
    bus_message_out = 35222,
    nav_direct_in   = 25221
};
};

namespace ZMQ {
/**
  * @brief Хранит пути поиска файлов настроек.
  * @note Если понадобится подобное для других файлов настроек, то можно вынести отсюда и более универсализировать. Например передавая аргументом имя файла
  */
static const QStringList settings_search_paths = {
    SC::REL_CFG_PATH,
    "/opt/spoarmksim/config/"
};

/**
 * @brief Идентификатор отправителя для Heartbeat Журнала
 */
static const char* LOGS_HB_ID = "LOGS_HB";

/**
 * @brief Возвращает имя файла настроек ZMQ который был найден в путях поиска. Возвращает QString() если файл не найден.
 * @note Если понадобится подобное для других файлов настроек, то можно вынести отсюда и более универсализировать. Например передавая аргументом имя файла
 * @return
 */
inline QString getZMQSettingsFilename()
{
    QFileInfo settings_file_info;
    for(auto search_path: settings_search_paths ) {
        settings_file_info.setFile(search_path+"zmq.ini");
        if(settings_file_info.exists())
            break;
    }
    if(!settings_file_info.exists())
        return QString();
    return settings_file_info.filePath();
}
} // namespace ZMQ

/**
 * @brief returns ZMQ message created from anything thats QDataStream can accept.
 * @code
 * std::unique_ptr<zmq::message_t> message_key_event(prepareZMQMessage(key_event));
 * @endcode
 * @param message
 * @return unique_ptr to zmq::message_t
 */
template<typename T> inline std::unique_ptr<zmq::message_t> serializeToZMQMessage(const T& message)
{
    QByteArray buffer;
    QDataStream data_stream(&buffer, QIODevice::WriteOnly);
    data_stream.setVersion(I_QDATASTREAMVERSION);
    data_stream << message;
    auto zmq_message = std::make_unique<zmq::message_t>(buffer.size());
    memcpy(static_cast<void*>(zmq_message->data()), buffer.data(), buffer.size());
    return zmq_message;
}

/**
 * @brief returns object of type that accepts QDataStream created from ZMQ message.
 * @code
 * KeyEvent key_event(deserializeFromZMQMessage<KeyEvent>(message_keys_in));
 * @endcode
 * @param zmq_message ZMQ message
 * @return object of type that accepts QDataStream
 */
template<typename T> inline T deserializeFromZMQMessage(const zmq::message_t& zmq_message)
{
    QByteArray buffer(static_cast<const char*>(zmq_message.data()), zmq_message.size());
//    QByteArray buffer("asdasdasd3234234");
    QDataStream data_stream(&buffer, QIODevice::ReadOnly);
    data_stream.setVersion(I_QDATASTREAMVERSION);
    T message;
#if QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    data_stream.startTransaction();       // Since: Qt 5.7
    data_stream >> message;
    if(!data_stream.commitTransaction()) {
        //or raise exception or do something else?
        qCritical("deserializeFromZMQMessage<%s>: can't deserialize message to requested type", typeid(T).name());
    }
#else
    data_stream >> message;
#endif
    return message;
}

/**
 * @brief qStringFromEnvelope converts ZMQ envelope to QString.
 * @param message
 * @return
 */
inline QString qStringFromEnvelope(const zmq::message_t& message)
{
//    std::unique_ptr<char[]> sender(new char[message_envelope.size()]);  //NON-NULL-TERMINATED string!
//    memcpy(sender.get(), message_envelope.data(), message_envelope.size());
    return QString::fromUtf8(static_cast<const char*>(message.data()), message.size());
}

inline void connectZMQSocket(const std::unique_ptr<zmq::socket_t>& socket, const QString& address, int port)
{
    QString connection_string = "tcp://"
            % address
            % ":"
            % QString::number(port);
    try {
        socket->connect(connection_string.toLatin1().constData());
    } catch (const zmq::error_t& err) {
        QString error_string = QObject::tr("F0004: Can't connect socket to '%1'."
                                  "\nZMQ error code: '%2'."
                                  "\nZMQ error reason: '%3'.").arg(connection_string).arg(err.num()).arg(err.what());
        IF::criticalMessage(QObject::tr("Error"), error_string);
        IF::exitLater(1);
    } catch (const std::exception& exp){    //Остальные ловим отладчиком
        qFatal("F0005: Exception during connect socket to '%s'"
               "\n\texp.what(): '%s'.",
               qPrintable(connection_string),
               exp.what());
    } catch (...) {
        qFatal("F0006: Unhandled error during socket connect to '%s'! 'catch(...)'",
               qPrintable(connection_string));
    }
    qDebug("Socket connect to %s", qPrintable(connection_string));
}

inline void bindZMQSocket(const std::unique_ptr<zmq::socket_t>& socket, int port, const char* description="")
{
    char bind_buf[32] = {}; //Address fits in 32 bytes
    snprintf(bind_buf, 32, "tcp://*:%d", port);
    try {
        socket->bind(bind_buf);
    } catch (const zmq::error_t& err) {
        QString error_string = QObject::tr("F0001: Can't bind socket on port '%1'."
                                           "\nZMQ error code: '%2'."
                                           "\nZMQ error reason: '%3'.")
                .arg(port).arg(err.num()).arg(err.what());
        IF::criticalMessage(QObject::tr("Error"), error_string);
        IF::exitLater(1);
    } catch (const std::exception& exp){
        qFatal("F0002: Exception during bind socket on port '%d'"
               "\n\texp.what(): '%s'.",
               port,
               exp.what());
    } catch (...) {
        qFatal("F0003: Unhandled error during socket bind! on port '%d' 'catch(...)'", port);
    }
    qDebug("Socket '%-15s' bind on '%s'", description, bind_buf);
}

inline void checkResAndPrintZMQError(bool res, const char* where = "")
{
    if (!res) {
        qWarning("ZMQ Error in '%s': '%d:%s'.",
                 where, zmq_errno(), zmq_strerror(zmq_errno()));
    }
}

#endif // ZMQHELPERFUNCTIONS_H
