#include "native/glfw/KeyMod.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

using namespace nglpmt::native;

namespace {
    int toGlfw(const KeyMod& key_mod){
        switch (key_mod){
            case KeyMod::Shift: return GLFW_MOD_SHIFT;
            case KeyMod::Control: return GLFW_MOD_CONTROL;
            case KeyMod::Alt: return GLFW_MOD_ALT;
            case KeyMod::Super: return GLFW_MOD_SUPER;
            case KeyMod::CapsLock: return GLFW_MOD_CAPS_LOCK;
            case KeyMod::NumLock: return GLFW_MOD_NUM_LOCK;
            default: return 0;
        }
    }
};

KeyMods::KeyMods(const int& glfw_key_mods) :
    _mods(glfw_key_mods){
}

bool KeyMods::isMod(const KeyMod& mod){
    return _mods & toGlfw(mod);
}