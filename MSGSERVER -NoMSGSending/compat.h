#ifndef _COMPAT_H_
#define _COMPAT_H_

#include <QString>

#define ISOWinDateTime "yyyy-MM-ddTHH-mm-ss"
#define ISOWinDateTimeMs "yyyy-MM-ddTHH-mm-ss.zzz"

#ifndef WIN32
//#include <pthread.h>
#include <unistd.h> //usleep here
inline void msleep(unsigned int duration)
{
    usleep(duration*1000);
}
/**
 * @brief winFileName
 * @param input
 * @param replacement
 * @return
 */
inline QString winFileName(const QString& input, const QString& replacement = "-")
{
    Q_UNUSED(replacement)
    return QString(input);
}
#else
#include <QRegularExpression>
#include <windows.h>
#define msleep(x) Sleep(x)
/**
 * @brief Возвращает копию строки в которой запрещённые в именах файлов Windows символы заменены на '-' или на второй аргумент
 * @param input - строка для замены. Не будет изменена
 * @param replacement - изменённая копия строки.
 * @return
 */
inline QString winFileName(const QString& input, const QString& replacement = "-")
{
    QString out(input);
    QRegularExpression re("[:<>|?*]");
    out.replace(re, replacement);
    return out;
}
#endif


#endif // _COMPAT_H_
