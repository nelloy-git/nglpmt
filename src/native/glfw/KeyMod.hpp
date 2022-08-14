#pragma once

#include <string>

namespace nglpmt::native {

enum class KeyMod {
    Shift,
    Control,
    Alt,
    Super,
    CapsLock,
    NumLock,
    Unknown
};

struct KeyMods {
    KeyMods(const int& glfw_key_mods);
    bool isMod(const KeyMod& mod);

private:
    int _mods;
};

}