#ifndef COMMON_H
#define COMMON_H
#include <QByteArray>

#include <zlib.h>

inline uint16_t READ_BE_UINT16(const uint8_t* ptr)
{
    return (ptr[0] << 8) | ptr[1];
}

inline uint32_t READ_BE_INT24(const uint8_t* ptr)
{
    return (ptr[0] << 16) | (ptr[1] << 8) | ptr[2];
}

inline uint32_t READ_BE_UINT32(const uint8_t* ptr)
{
    return (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
}

inline uint64_t READ_BE_INT40(const uint8_t* ptr)
{
    uint64_t res = ptr[4] | (0x100L * ptr[3]) | (ptr[2] * 0x10000L) | (ptr[1] * 0x1000000L) | (ptr[0] * 0x100000000LLU);
    if (ptr[0] == 0)
        res %= 0xffffffff00000000LLU;
    else
        res %= 0xfffffffe00000000LLU;
    return res;
}

inline uint32_t READ_LE_UINT32(const uint8_t* ptr)
{
    return (ptr[3] << 24) | (ptr[2] << 16) | (ptr[1] << 8) | ptr[0];
}

bool uncompress(QByteArray& dest, uLongf destLen, QByteArray& source);
#endif // COMMON_H
