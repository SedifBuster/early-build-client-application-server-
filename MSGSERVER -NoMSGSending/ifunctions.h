/* -*- C -*-
 * FILE: "D:\project\ikar2\include\ifunctions.h"
 * LAST MODIFICATION: "2014-02-24 14:46:22 Московское время (зима) (misha)"
 * (C) 2014 by Bryukhovets Mikhail, <misha@bryukhovets.ru>
 * vim:set sts=4 ts=4 sw=4:
 * $Id:$
 */

#ifndef IFUNCTIONS_H
#define IFUNCTIONS_H

#include <QCoreApplication>
#include <QTimer>

#ifdef QT_GUI_LIB
#include <QMessageBox>
#endif

#include <algorithm>
#include <climits>
#include <cmath>
#include <cstddef>
#include <functional>
#include <utility>

namespace IF {


inline void exitLater(int ret=0)
{
#if QT_VERSION >= 0x050000
    QTimer::singleShot(0, [ret](){QCoreApplication::exit(ret);});
#else
    QTimer::singleShot(0, QCoreApplication::instance(), SLOT(exit(ret)));   // Qt4 compatible
#endif
}


/**
 * returns number of elements of any array
 * Not for variable-length arrays
 * Not for pointers
 * @todo: probably rename to lengthOfArray? If assume that size is in bytes and lenght is in members
 * @note: Note that `int items [] = {1, 2, 3}` is a fixed size aray
 * @code
 * int items[] = {1, 2, 3};
 * int values[20] = {0};
 * int y[5] = {1,2,3};
 * cout << sizeOfArray(items);   //3
 * cout << sizeOfArray(values);  //20
 * cout << sizeOfArray(y);       //5
 * @endcode
 */
template <typename T, size_t N> inline
constexpr size_t sizeOfArray( const T(&)[ N ] ) noexcept
{
    return N;
}

/**
 * @brief B2S converts bool to string representation
 * @param param boolean param
 * @return "true" if \a param is true, otherwhise returns "false"
 */
inline const char* B2S(bool param)
{
    return param ? "true" : "false";
}

/**
 * @brief 64-bit representation of memset
 * code from http://stackoverflow.com/questions/108866/is-there-memset-that-accepts-integers-larger-than-char
 * @param dest
 * @param value
 * @param size
 */
//inline void memset64( void * dest, uint64_t value, uintptr_t size )
//{
//  uintptr_t i;
//  for( i = 0; i < (size & (~7)); i+=8 )
//  {
//    memcpy( ((char*)dest) + i, &value, 8 );
//  }
//  for( ; i < size; i++ )
//  {
//    ((char*)dest)[i] = ((char*)&value)[i&7];
//  }
//}

/** delete object and set pointer to 0
 * from http://www.stroustrup.com/bs_faq2.html
 */
template<class T> inline void destroy(T*& p)
{ delete p; p = nullptr; }


//FIXME
template<class T> inline
bool check_range(T value, T min, T max) {
    return (value >= min) && (value <= max);
}

// basic circular right shift. No safety checks!
template<class T>
T rotr(T value, unsigned int shift)
{
    static_assert (std::is_unsigned<T>::value,
                   "Rotate support only unsigned value");
    shift %= std::numeric_limits<T>::digits;
    return (value >> shift) | (value << std::numeric_limits<T>::digits - shift);
}

// basic circular left shift. No safety checks!
template<class T>
T rotl(T value, unsigned int shift)
{
    static_assert (std::is_unsigned<T>::value,
                   "Rotate support only unsigned value");
    shift %= std::numeric_limits<T>::digits;
    return (value << shift) | (value >> std::numeric_limits<T>::digits - shift);
}

/**
 * Gereic solution to determine number of digits in integer part of number.
 * Should be faster then log10 for most cases
 * @note Exactly 0 number counts as 0 digits
 */
template <class T>
int numDigits(T number)
{
    static_assert (std::numeric_limits<T>::is_integer, "Number of digits can be calculated only for integer values");
    int digits = 0;
//    if (number < 0) // '-' counts as digit
//        digits = 1;
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}

inline void criticalMessage(const QString title, const QString message)
{
#ifdef QT_GUI_LIB
    QMessageBox::critical(nullptr, title, message, QMessageBox::Ok);
#endif
    qCritical("%s: %s", qPrintable(title), qPrintable(message));
}

}   // namespace IF

#endif  //IFUNCTIONS_H
