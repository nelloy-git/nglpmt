#include "native/gl/Object.hpp"

#include "glad/gl.h"

using namespace nglpmt::native;

std::optional<std::string> GlObjectStatic::getGlMessage(const SrcLoc& src_loc){
    Enum err = glGetError();
    if (err == GL_NO_ERROR){
        return std::nullopt;
    }

    std::string msg = "==========\nDebug:";
    while (err != GL_NO_ERROR){
        std::string err_name;
        switch (err){
            case GL_INVALID_ENUM: err_name = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE: err_name = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: err_name = "GL_INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW: err_name = "GL_STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW: err_name = "GL_STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY: err_name = "GL_OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: err_name = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_CONTEXT_LOST: err_name = "GL_CONTEXT_LOST"; break;
            default: err_name = "UNKNOWN";
        }
        
        msg += "\n" + src_loc.to_string() + "\t" + err_name + "(" + std::to_string(err) + ")";        
        err = glGetError();
    }
    msg += "\n==========";
    return msg;
}