#include "native/gl/Shader.hpp"

#include "glad/gl.h"

using namespace nglpmt::native;

std::shared_ptr<GlShader> GlShader::make(const std::shared_ptr<Context>& ctx,
                                         const Val<const Enum>& type,
                                         const Val<const SrcLoc>& src_loc){
    return std::shared_ptr<GlShader>(new GlShader(ctx, type, src_loc));
}

GlShader::GlShader(const std::shared_ptr<Context>& ctx,
                   const Val<const Enum>& type,
                   const Val<const SrcLoc>& src_loc) :
    GlObject(ctx, GlShader::_initer, GlShader::_deleter, type, src_loc){
}
           
void GlShader::getParameteriv(const Val<const Enum>& pname,
                              const Val<Int[]>& params,
                              const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlShader::getParameteriv>(pname, params, src_loc)){return;}
    glGetShaderiv(id(), pname, params);
    debug(src_loc);
}

void GlShader::getType(const Val<Enum>& dst,
                       const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_SHADER_TYPE, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlShader::isCompiled(const Val<bool>& dst,
                          const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_COMPILE_STATUS, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlShader::getSourceLength(const Val<Int>& dst,
                               const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_SHADER_SOURCE_LENGTH, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlShader::getInfoLog(const Val<std::string>& infoLog,
                          const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlShader::getInfoLog>(infoLog, src_loc)){return;}
    Int length;
    glGetShaderiv(id(), GL_INFO_LOG_LENGTH, &length);
    infoLog->reserve(length);
    glGetShaderInfoLog(id(), length, &length, infoLog->data());
    debug(src_loc);
}

void GlShader::source(const Val<const std::string>& code,
                         const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlShader::source>(code, src_loc)){return;}
    auto c_code = code->c_str();
    glShaderSource(id(), 1, &c_code, nullptr);
    debug(src_loc);
}

void GlShader::compile(const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlShader::compile>(src_loc)){return;}
    glCompileShader(id());
    debug(src_loc);
}

void GlShader::_initer(const Val<UInt>& dst,
                       const Val<const Enum>& type,
                       const Val<const SrcLoc>& src_loc){
    *dst = glCreateShader(type);
    debug(src_loc);
}

void GlShader::_deleter(const UInt& id){
    glDeleteShader(id);
}
