#ifndef BENDDATA_H
#define BENDDATA_H

#include <cstdint>

#pragma pack(push, 1)
struct Bend
{
    float    m_time;
    float    m_step;
    uint16_t m_unknown0;
    uint8_t  m_unknown1;
    uint8_t  m_unknown2;
};

struct Bend32
{
    Bend     m_bends[32];
    uint32_t m_count;
};
#pragma pack(pop)

#endif // BENDDATA_H
