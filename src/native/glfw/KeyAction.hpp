#pragma once

#include <string>

namespace nglpmt::native {

enum class KeyAction {
    Release,
    Press,
    Repeat,
    Unknown
};

KeyAction glfwToKeyAction(const int& glfw_key_action);
std::string keyActionToString(const KeyAction& key_action);
    
} // namespace glwpp
