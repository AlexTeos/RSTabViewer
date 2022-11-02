#include "noteeffects.h"

namespace RS
{
namespace Effects
{
bool isChord(uint32_t mask)
{
    return mask & Flags::Chord;
}

bool isTremolo(uint32_t mask)
{
    return mask & Flags::Tremolo;
}

bool isHarmonic(uint32_t mask)
{
    return mask & Flags::Harmonic;
}
bool isPalmMute(uint32_t mask)
{
    return mask & Flags::PalmMute;
}

bool isHammerOn(uint32_t mask)
{
    return mask & Flags::HammerOn;
}

bool isPullOff(uint32_t mask)
{
    return mask & Flags::PullOff;
}
bool isSlide(uint32_t mask)
{
    return mask & Flags::Slide;
}
bool isBend(uint32_t mask)
{
    return mask & Flags::Bend;
}

bool isSustain(uint32_t mask)
{
    return mask & Flags::Sustain;
}

bool isPinchHarmonic(uint32_t mask)
{
    return mask & Flags::PinchHarmonic;
}

bool isVibrato(uint32_t mask)
{
    return mask & Flags::Vibrato;
}

bool isMute(uint32_t mask)
{
    return mask & Flags::Mute || mask & Flags::FretHandMute;
}

bool isUnpitchedSlide(uint32_t mask)
{
    return mask & Flags::UnpitchedSlide;
}
bool isSingle(uint32_t mask)
{
    return mask & Flags::Single;
}

bool isChordNotes(uint32_t mask)
{
    return mask & Flags::ChordNotes;
}
bool isAccent(uint32_t mask)
{
    return mask & Flags::Accent;
}

bool isParent(uint32_t mask)
{
    return mask & Flags::Parent;
}

bool isChild(uint32_t mask)
{
    return mask & Flags::Child;
}

}
}
