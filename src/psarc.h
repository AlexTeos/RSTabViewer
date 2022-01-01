/*
 * Open PSARC PS3 extractor
 * Copyright (C) 2011-2018 Matthieu Milan
 */

#ifndef PSARC_H__
#define PSARC_H__

#include <QFile>
#include <QString>
#include <QVector>

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

struct PSARCEntry
{
    int32_t  _id;
    uint64_t _length;
    QString  _name;
    uint32_t _zIndex;
    uint64_t _zOffset;
};

class PSARC
{
public:
    bool unarchive(const QString& archiveName);

private:
    const uint32_t headerSize        = 20;
    const uint32_t kPSARCMagicNumber = 0x50534152;

    bool uncompress(QByteArray& dest, uLongf destLen, QByteArray& source);
    bool inflateEntry(uint32_t& entry, QVector<uint32_t>& zBlocks, uint32_t& cBlockSize, QString fileName, QFile& file);

    QFile               _f;
    QVector<PSARCEntry> _entries;
};

#endif // PSARC_H__
