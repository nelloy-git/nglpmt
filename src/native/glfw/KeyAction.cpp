#include "native/glfw/KeyAction.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

using namespace nglpmt;

native::KeyAction native::glfwToKeyAction(const int& glfw_key_action){
    switch (glfw_key_action){
        case GLFW_PRESS: return KeyAction::Press;
        case GLFW_RELEASE: return KeyAction::Release;
        case GLFW_REPEAT: return KeyAction::Repeat;
        default: return KeyAction::Unknown;
    }
}

std::string native::keyActionToString(const KeyAction& key_action){
    switch (key_action){
        case KeyAction::Press: return "Press";
        case KeyAction::Release: return "Release";
        case KeyAction::Repeat: return "Repeat";
        default: return "Unknown";
    }
}