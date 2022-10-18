#include "native/gl/Program.hpp"

#include "glad/gl.h"

using namespace nglpmt::native;

std::shared_ptr<GlProgram> GlProgram::make(const std::shared_ptr<Context>& ctx,
                            const Val<const SrcLoc>& src_loc){
    return std::shared_ptr<GlProgram>(new GlProgram(ctx, src_loc));
}

GlProgram::GlProgram(const std::shared_ptr<Context>& ctx,
                     const Val<const SrcLoc>& src_loc) :
    GlObject(ctx, &GlProgram::_initer, &GlProgram::_deleter, src_loc){
}

void GlProgram::getParameteriv(const Val<const Enum>& pname,
                               const Val<Int[]>& params,
                               const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlProgram::getParameteriv>(pname, params, src_loc)){return;}
    glGetProgramiv(id(), pname, params);
    debug(src_loc);
}

void GlProgram::isLinked(const Val<bool>& dst,
                         const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_LINK_STATUS, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlProgram::isValidated(const Val<bool>& dst,
                            const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_VALIDATE_STATUS, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlProgram::getAttachedShadersCount(const Val<Int>& dst,
                                        const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_ATTACHED_SHADERS, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlProgram::getActiveAttributesCount(const Val<Int>& dst,
                                         const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_ACTIVE_ATTRIBUTES, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlProgram::getActiveAttributeMaxNameLength(const Val<Int>& dst,
                                                const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlProgram::getActiveUniformsCount(const Val<Int>& dst,
                                       const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_ACTIVE_UNIFORMS, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlProgram::getActiveUniformMaxNameLength(const Val<Int>& dst,
                                              const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_ACTIVE_UNIFORM_MAX_LENGTH, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlProgram::getInfoLog(const Val<std::string>& infoLog,
                           const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlProgram::getInfoLog>(infoLog, src_loc)){return;}
    Int length;
    glGetProgramiv(id(), GL_INFO_LOG_LENGTH, &length);
    infoLog->resize(length);
    glGetProgramInfoLog(id(), length, &length, infoLog->data());
    debug(src_loc);
}

void GlProgram::getAttributeLocation(const Val<Int>& dst,
                                     const Val<const std::string>& name,
                                     const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlProgram::getAttributeLocation>(dst, name, src_loc)){return;}
    *dst = glGetAttribLocation(id(), name->c_str());
    debug(src_loc);
}

void GlProgram::bindAttributeLocation(const Val<const UInt>& index,
                                      const Val<const std::string>& name,
                                      const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlProgram::bindAttributeLocation>(index, name, src_loc)){return;}
    glBindAttribLocation(id(), index, name->c_str());
    debug(src_loc);
}

void GlProgram::getUniformLocation(const Val<Int>& dst,
                                   const Val<const std::string>& name,
                                   const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlProgram::getUniformLocation>(dst, name, src_loc)){return;}
    *dst = glGetUniformLocation(id(), name->c_str());
    debug(src_loc);
}

void GlProgram::getUniformBlockIndex(const Val<UInt>& dst,
                                     const Val<const std::string>& name,
                                     const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlProgram::getUniformBlockIndex>(dst, name, src_loc)){return;}
    glGetUniformBlockIndex(id(), name->c_str());
    debug(src_loc);
}

void GlProgram::attach(const Val<const GlShader>& shader,
                       const Val<const SrcLoc>& src_loc) {
    if (movedToContext<&GlProgram::attach>(shader, src_loc)){return;}
    glAttachShader(id(), shader->id());
    debug(src_loc);
}

void GlProgram::link(const Val<const SrcLoc>& src_loc) {
    if (movedToContext<&GlProgram::link>(src_loc)){return;}
    glLinkProgram(id());
    debug(src_loc);
}

void GlProgram::validate(const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlProgram::validate>(src_loc)){return;}
    glValidateProgram(id());
    debug(src_loc);
}

void GlProgram::use(const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlProgram::use>(src_loc)){return;}
    glUseProgram(id());
    debug(src_loc);
}

    
void GlProgram::uniform1f(const Val<const Int>& location,
                          const Val<const Float>& v0,
                          const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform1f>(location, v0, src_loc)){return;}
    glProgramUniform1f(id(), location, v0);
    debug(src_loc);
}

void GlProgram::uniform2f(const Val<const Int>& location,
                          const Val<const Float>& v0,
                          const Val<const Float>& v1,
                          const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform2f>(location, v0, v1, src_loc)){return;}
    glProgramUniform2f(id(), location, v0, v1);
    debug(src_loc);
}

void GlProgram::uniform3f(const Val<const Int>& location,
                          const Val<const Float>& v0,
                          const Val<const Float>& v1,
                          const Val<const Float>& v2,
                          const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform3f>(location, v0, v1, v2, src_loc)){return;}
    glProgramUniform3f(id(), location, v0, v1, v2);
    debug(src_loc);
}

void GlProgram::uniform4f(const Val<const Int>& location,
                          const Val<const Float>& v0,
                          const Val<const Float>& v1,
                          const Val<const Float>& v2,
                          const Val<const Float>& v3,
                          const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform4f>(location, v0, v1, v2, v3, src_loc)){return;}
    glProgramUniform4f(id(), location, v0, v1, v2, v3);
    debug(src_loc);
}

void GlProgram::uniform1fv(const Val<const Int>& location,
                           const Val<const Sizei>& count,
                           const Val<const Float[]>& data,
                           const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform1fv>(location, count, data, src_loc)){return;}
    glProgramUniform1fv(id(), location, count, data);
    debug(src_loc);
}

void GlProgram::uniform2fv(const Val<const Int>& location,
                           const Val<const Sizei>& count,
                           const Val<const Float[]>& data,
                           const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform2fv>(location, count, data, src_loc)){return;}
    glProgramUniform2fv(id(), location, count, data);
    debug(src_loc);
}

void GlProgram::uniform3fv(const Val<const Int>& location,
                           const Val<const Sizei>& count,
                           const Val<const Float[]>& data,
                           const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform3fv>(location, count, data, src_loc)){return;}
    glProgramUniform3fv(id(), location, count, data);
    debug(src_loc);
}

void GlProgram::uniform4fv(const Val<const Int>& location,
                           const Val<const Sizei>& count,
                           const Val<const Float[]>& data,
                           const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform4fv>(location, count, data, src_loc)){return;}
    glProgramUniform4fv(id(), location, count, data);
    debug(src_loc);
}

void GlProgram::uniformMatrix2fv(const Val<const Int>& location,
                                 const Val<const Sizei>& count,
                                 const Val<const bool>& transpose,
                                 const Val<const Float[]>& data,
                                 const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniformMatrix2fv>(location, count, transpose, data, src_loc)){return;}
    glProgramUniformMatrix2fv(id(), location, count, transpose, data);
    debug(src_loc);
}

void GlProgram::uniformMatrix2x3fv(const Val<const Int>& location,
                                   const Val<const Sizei>& count,
                                   const Val<const bool>& transpose,
                                   const Val<const Float[]>& data,
                                   const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniformMatrix2x3fv>(location, count, transpose, data, src_loc)){return;}
    glProgramUniformMatrix2x3fv(id(), location, count, transpose, data);
    debug(src_loc);
}

void GlProgram::uniformMatrix2x4fv(const Val<const Int>& location,
                                   const Val<const Sizei>& count,
                                   const Val<const bool>& transpose,
                                   const Val<const Float[]>& data,
                                   const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniformMatrix2x4fv>(location, count, transpose, data, src_loc)){return;}
    glProgramUniformMatrix2x4fv(id(), location, count, transpose, data);
    debug(src_loc);
}

void GlProgram::uniformMatrix3fv(const Val<const Int>& location,
                                 const Val<const Sizei>& count,
                                 const Val<const bool>& transpose,
                                 const Val<const Float[]>& data,
                                 const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniformMatrix3fv>(location, count, transpose, data, src_loc)){return;}
    glProgramUniformMatrix3fv(id(), location, count, transpose, data);
    debug(src_loc);
}

void GlProgram::uniformMatrix3x2fv(const Val<const Int>& location,
                                   const Val<const Sizei>& count,
                                   const Val<const bool>& transpose,
                                   const Val<const Float[]>& data,
                                   const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniformMatrix3x2fv>(location, count, transpose, data, src_loc)){return;}
    glProgramUniformMatrix3x2fv(id(), location, count, transpose, data);
    debug(src_loc);
}

void GlProgram::uniformMatrix3x4fv(const Val<const Int>& location,
                                   const Val<const Sizei>& count,
                                   const Val<const bool>& transpose,
                                   const Val<const Float[]>& data,
                                   const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniformMatrix3x4fv>(location, count, transpose, data, src_loc)){return;}
    glProgramUniformMatrix3x4fv(id(), location, count, transpose, data);
    debug(src_loc);
}

void GlProgram::uniformMatrix4fv(const Val<const Int>& location,
                                 const Val<const Sizei>& count,
                                 const Val<const bool>& transpose,
                                 const Val<const Float[]>& data,
                                 const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniformMatrix4fv>(location, count, transpose, data, src_loc)){return;}
    glProgramUniformMatrix4fv(id(), location, count, transpose, data);
    debug(src_loc);
}

void GlProgram::uniformMatrix4x2fv(const Val<const Int>& location,
                                   const Val<const Sizei>& count,
                                   const Val<const bool>& transpose,
                                   const Val<const Float[]>& data,
                                   const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniformMatrix4x2fv>(location, count, transpose, data, src_loc)){return;}
    glProgramUniformMatrix4x2fv(id(), location, count, transpose, data);
    debug(src_loc);
}

void GlProgram::uniformMatrix4x3fv(const Val<const Int>& location,
                                   const Val<const Sizei>& count,
                                   const Val<const bool>& transpose,
                                   const Val<const Float[]>& data,
                                   const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniformMatrix4x3fv>(location, count, transpose, data, src_loc)){return;}
    glProgramUniformMatrix4x3fv(id(), location, count, transpose, data);
    debug(src_loc);
}
    
void GlProgram::uniform1d(const Val<const Int>& location,
                          const Val<const Double>& v0,
                          const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform1d>(location, v0, src_loc)){return;}
    glProgramUniform1d(id(), location, v0);
    debug(src_loc);
}

void GlProgram::uniform2d(const Val<const Int>& location,
                          const Val<const Double>& v0,
                          const Val<const Double>& v1,
                          const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform2d>(location, v0, v1, src_loc)){return;}
    glProgramUniform2d(id(), location, v0, v1);
    debug(src_loc);
}

void GlProgram::uniform3d(const Val<const Int>& location,
                          const Val<const Double>& v0,
                          const Val<const Double>& v1,
                          const Val<const Double>& v2,
                          const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform3d>(location, v0, v1, v2, src_loc)){return;}
    glProgramUniform3d(id(), location, v0, v1, v2);
    debug(src_loc);
}

void GlProgram::uniform4d(const Val<const Int>& location,
                          const Val<const Double>& v0,
                          const Val<const Double>& v1,
                          const Val<const Double>& v2,
                          const Val<const Double>& v3,
                          const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform4d>(location, v0, v1, v2, v3, src_loc)){return;}
    glProgramUniform4d(id(), location, v0, v1, v2, v3);
    debug(src_loc);
}

void GlProgram::uniform1dv(const Val<const Int>& location,
                           const Val<const Sizei>& count,
                           const Val<const Double[]>& data,
                           const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform1dv>(location, count, data, src_loc)){return;}
    glProgramUniform1dv(id(), location, count, data);
    debug(src_loc);
}

void GlProgram::uniform2dv(const Val<const Int>& location,
                           const Val<const Sizei>& count,
                           const Val<const Double[]>& data,
                           const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform2dv>(location, count, data, src_loc)){return;}
    glProgramUniform2dv(id(), location, count, data);
    debug(src_loc);
}

void GlProgram::uniform3dv(const Val<const Int>& location,
                           const Val<const Sizei>& count,
                           const Val<const Double[]>& data,
                           const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform3dv>(location, count, data, src_loc)){return;}
    glProgramUniform3dv(id(), location, count, data);
    debug(src_loc);
}

void GlProgram::uniform4dv(const Val<const Int>& location,
                           const Val<const Sizei>& count,
                           const Val<const Double[]>& data,
                           const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform4dv>(location, count, data, src_loc)){return;}
    glProgramUniform4dv(id(), location, count, data);
    debug(src_loc);
}

void GlProgram::uniformMatrix2dv(const Val<const Int>& location,
                                 const Val<const Sizei>& count,
                                 const Val<const bool>& transpose,
                                 const Val<const Double[]>& data,
                                 const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniformMatrix2dv>(location, count, transpose, data, src_loc)){return;}
    glProgramUniformMatrix2dv(id(), location, count, transpose, data);
    debug(src_loc);
}

void GlProgram::uniformMatrix2x3dv(const Val<const Int>& location,
                                   const Val<const Sizei>& count,
                                   const Val<const bool>& transpose,
                                   const Val<const Double[]>& data,
                                   const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniformMatrix2x3dv>(location, count, transpose, data, src_loc)){return;}
    glProgramUniformMatrix2x3dv(id(), location, count, transpose, data);
    debug(src_loc);
}

void GlProgram::uniformMatrix2x4dv(const Val<const Int>& location,
                                   const Val<const Sizei>& count,
                                   const Val<const bool>& transpose,
                                   const Val<const Double[]>& data,
                                   const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniformMatrix2x4dv>(location, count, transpose, data, src_loc)){return;}
    glProgramUniformMatrix2x4dv(id(), location, count, transpose, data);
    debug(src_loc);
}

void GlProgram::uniformMatrix3dv(const Val<const Int>& location,
                                 const Val<const Sizei>& count,
                                 const Val<const bool>& transpose,
                                 const Val<const Double[]>& data,
                                 const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniformMatrix3dv>(location, count, transpose, data, src_loc)){return;}
    glProgramUniformMatrix3dv(id(), location, count, transpose, data);
    debug(src_loc);
}

void GlProgram::uniformMatrix3x2dv(const Val<const Int>& location,
                                   const Val<const Sizei>& count,
                                   const Val<const bool>& transpose,
                                   const Val<const Double[]>& data,
                                   const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniformMatrix3x2dv>(location, count, transpose, data, src_loc)){return;}
    glProgramUniformMatrix3x2dv(id(), location, count, transpose, data);
    debug(src_loc);
}

void GlProgram::uniformMatrix3x4dv(const Val<const Int>& location,
                                   const Val<const Sizei>& count,
                                   const Val<const bool>& transpose,
                                   const Val<const Double[]>& data,
                                   const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniformMatrix3x4dv>(location, count, transpose, data, src_loc)){return;}
    glProgramUniformMatrix3x4dv(id(), location, count, transpose, data);
    debug(src_loc);
}

void GlProgram::uniformMatrix4dv(const Val<const Int>& location,
                                 const Val<const Sizei>& count,
                                 const Val<const bool>& transpose,
                                 const Val<const Double[]>& data,
                                 const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniformMatrix4dv>(location, count, transpose, data, src_loc)){return;}
    glProgramUniformMatrix4dv(id(), location, count, transpose, data);
    debug(src_loc);
}

void GlProgram::uniformMatrix4x2dv(const Val<const Int>& location,
                                   const Val<const Sizei>& count,
                                   const Val<const bool>& transpose,
                                   const Val<const Double[]>& data,
                                   const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniformMatrix4x2dv>(location, count, transpose, data, src_loc)){return;}
    glProgramUniformMatrix4x2dv(id(), location, count, transpose, data);
    debug(src_loc);
}

void GlProgram::uniformMatrix4x3dv(const Val<const Int>& location,
                                   const Val<const Sizei>& count,
                                   const Val<const bool>& transpose,
                                   const Val<const Double[]>& data,
                                   const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniformMatrix4x3dv>(location, count, transpose, data, src_loc)){return;}
    glProgramUniformMatrix4x3dv(id(), location, count, transpose, data);
    debug(src_loc);
}

void GlProgram::uniform1i(const Val<const Int>& location,
                          const Val<const Int>& v0,
                          const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform1i>(location, v0, src_loc)){return;}
    glProgramUniform1i(id(), location, v0);
    debug(src_loc);
}

void GlProgram::uniform2i(const Val<const Int>& location,
                          const Val<const Int>& v0,
                          const Val<const Int>& v1,
                          const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform2i>(location, v0, v1, src_loc)){return;}
    glProgramUniform2i(id(), location, v0, v1);
    debug(src_loc);
}

void GlProgram::uniform3i(const Val<const Int>& location,
                          const Val<const Int>& v0,
                          const Val<const Int>& v1,
                          const Val<const Int>& v2,
                          const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform3i>(location, v0, v1, v2, src_loc)){return;}
    glProgramUniform3i(id(), location, v0, v1, v2);
    debug(src_loc);
}

void GlProgram::uniform4i(const Val<const Int>& location,
                          const Val<const Int>& v0,
                          const Val<const Int>& v1,
                          const Val<const Int>& v2,
                          const Val<const Int>& v3,
                          const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform4i>(location, v0, v1, v2, v3, src_loc)){return;}
    glProgramUniform4i(id(), location, v0, v1, v2, v3);
    debug(src_loc);
}

void GlProgram::uniform1iv(const Val<const Int>& location,
                           const Val<const Sizei>& count,
                           const Val<const Int[]>& data,
                           const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform1iv>(location, count, data, src_loc)){return;}
    glProgramUniform1iv(id(), location, count, data);
    debug(src_loc);
}

void GlProgram::uniform2iv(const Val<const Int>& location,
                           const Val<const Sizei>& count,
                           const Val<const Int[]>& data,
                           const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform2iv>(location, count, data, src_loc)){return;}
    glProgramUniform2iv(id(), location, count, data);
    debug(src_loc);
}

void GlProgram::uniform3iv(const Val<const Int>& location,
                           const Val<const Sizei>& count,
                           const Val<const Int[]>& data,
                           const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform3iv>(location, count, data, src_loc)){return;}
    glProgramUniform3iv(id(), location, count, data);
    debug(src_loc);
}

void GlProgram::uniform4iv(const Val<const Int>& location,
                           const Val<const Sizei>& count,
                           const Val<const Int[]>& data,
                           const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform4iv>(location, count, data, src_loc)){return;}
    glProgramUniform4iv(id(), location, count, data);
    debug(src_loc);
}

void GlProgram::uniform1ui(const Val<const Int>& location,
                           const Val<const UInt>& v0,
                           const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform1ui>(location, v0, src_loc)){return;}
    glProgramUniform1ui(id(), location, v0);
    debug(src_loc);
}

void GlProgram::uniform2ui(const Val<const Int>& location,
                           const Val<const UInt>& v0,
                           const Val<const UInt>& v1,
                           const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform2ui>(location, v0, v1, src_loc)){return;}
    glProgramUniform2ui(id(), location, v0, v1);
    debug(src_loc);
}

void GlProgram::uniform3ui(const Val<const Int>& location,
                           const Val<const UInt>& v0,
                           const Val<const UInt>& v1,
                           const Val<const UInt>& v2,
                           const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform3ui>(location, v0, v1, v2, src_loc)){return;}
    glProgramUniform3ui(id(), location, v0, v1, v2);
    debug(src_loc);
}

void GlProgram::uniform4ui(const Val<const Int>& location,
                           const Val<const UInt>& v0,
                           const Val<const UInt>& v1,
                           const Val<const UInt>& v2,
                           const Val<const UInt>& v3,
                           const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform4ui>(location, v0, v1, v2, v3, src_loc)){return;}
    glProgramUniform4ui(id(), location, v0, v1, v2, v3);
    debug(src_loc);
}

void GlProgram::uniform1uiv(const Val<const Int>& location,
                            const Val<const Sizei>& count,
                            const Val<const UInt[]>& data,
                            const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform1uiv>(location, count, data, src_loc)){return;}
    glProgramUniform1uiv(id(), location, count, data);
    debug(src_loc);
}

void GlProgram::uniform2uiv(const Val<const Int>& location,
                            const Val<const Sizei>& count,
                            const Val<const UInt[]>& data,
                            const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform2uiv>(location, count, data, src_loc)){return;}
    glProgramUniform2uiv(id(), location, count, data);
    debug(src_loc);
}

void GlProgram::uniform3uiv(const Val<const Int>& location,
                            const Val<const Sizei>& count,
                            const Val<const UInt[]>& data,
                            const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform3uiv>(location, count, data, src_loc)){return;}
    glProgramUniform3uiv(id(), location, count, data);
    debug(src_loc);
}

void GlProgram::uniform4uiv(const Val<const Int>& location,
                            const Val<const Sizei>& count,
                            const Val<const UInt[]>& data,
                            const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlProgram::uniform4uiv>(location, count, data, src_loc)){return;}
    glProgramUniform4uiv(id(), location, count, data);
    debug(src_loc);
}

void GlProgram::_initer(const Val<UInt>& dst,
                      const Val<const SrcLoc>& src_loc){
    *dst = glCreateProgram();
    debug(src_loc);
}

void GlProgram::_deleter(const UInt& id){
    glDeleteProgram(id);
}