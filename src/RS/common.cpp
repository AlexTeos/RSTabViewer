#include "common.h"

bool uncompress(QByteArray& dest, uLongf destLen, QByteArray& source)
{
    uLongf     destLenFlip = READ_BE_UINT32((uint8_t*)&destLen);
    QByteArray arr;
    arr.append((char*)&destLenFlip, 4);
    arr.append(source);
    dest = qUncompress(arr);
    return true;
}
