#ifndef NOTEEFFECTS_H
#define NOTEEFFECTS_H

#include <cstdint>
namespace RS
{
namespace Effects
{
namespace Flags
{
constexpr uint32_t Chord          = 0x00000002;
constexpr uint32_t FretHandMute   = 0x00000008;
constexpr uint32_t Tremolo        = 0x00000010;
constexpr uint32_t Harmonic       = 0x00000020;
constexpr uint32_t PalmMute       = 0x00000040;
constexpr uint32_t HammerOn       = 0x00000200;
constexpr uint32_t PullOff        = 0x00000400;
constexpr uint32_t Slide          = 0x00000800;
constexpr uint32_t Bend           = 0x00001000;
constexpr uint32_t Sustain        = 0x00002000;
constexpr uint32_t PinchHarmonic  = 0x00008000;
constexpr uint32_t Vibrato        = 0x00010000;
constexpr uint32_t Mute           = 0x00020000;
constexpr uint32_t UnpitchedSlide = 0x00400000;
constexpr uint32_t Single         = 0x00800000;
constexpr uint32_t ChordNotes     = 0x01000000;
constexpr uint32_t Accent         = 0x04000000;
constexpr uint32_t Parent         = 0x08000000;
constexpr uint32_t Child          = 0x10000000;
};

bool isChord(uint32_t mask);
bool isTremolo(uint32_t mask);
bool isHarmonic(uint32_t mask);
bool isPalmMute(uint32_t mask);
bool isHammerOn(uint32_t mask);
bool isPullOff(uint32_t mask);
bool isSlide(uint32_t mask);
bool isBend(uint32_t mask);
bool isSustain(uint32_t mask);
bool isPinchHarmonic(uint32_t mask);
bool isVibrato(uint32_t mask);
bool isMute(uint32_t mask);
bool isUnpitchedSlide(uint32_t mask);
bool isSingle(uint32_t mask);
bool isChordNotes(uint32_t mask);
bool isAccent(uint32_t mask);
bool isParent(uint32_t mask);
bool isChild(uint32_t mask);
}
}
#endif // NOTEEFFECTS_H
