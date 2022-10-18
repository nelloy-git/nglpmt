#include "native/gl/Buffer.hpp"

#include "glad/gl.h"

using namespace nglpmt::native;

std::shared_ptr<GlBuffer> GlBuffer::make(const std::shared_ptr<Context>& ctx,
                                         const Val<const SrcLoc>& src_loc){
    return std::shared_ptr<GlBuffer>(new GlBuffer(ctx, src_loc));
}

GlBuffer::GlBuffer(const std::shared_ptr<Context>& ctx, const Val<const SrcLoc>& src_loc) :
    GlObject(ctx, &GlBuffer::_initer, &GlBuffer::_deleter, src_loc){
}

GlBuffer::~GlBuffer(){
}

void GlBuffer::data(const Val<const SizeiPtr>& size,
                    const Val<const void>& data,
                    const Val<const Enum>& usage,
                    const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlBuffer::data>(size, data, usage, src_loc)){return;}
    std::cout << "Created: " << *usage << std::endl;
    glNamedBufferData(id(), size, data, usage);
    debug(src_loc);
}

void GlBuffer::storage(const Val<const SizeiPtr>& size,
                       const Val<const void>& data,
                       const Val<const BitField>& flags,
                       const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlBuffer::storage>(size, data, flags, src_loc)){return;}
    glNamedBufferStorage(id(), size, data, flags);
    debug(src_loc);
}

void GlBuffer::bindBase(const Val<const Enum>& target,
                        const Val<const UInt>& index,
                        const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlBuffer::bindBase>(target, index, src_loc)){return;}
    glBindBufferBase(target, index, id());
    debug(src_loc);
}

void GlBuffer::bindRange(const Val<const Enum>& target,
                         const Val<const UInt>& index,
                         const Val<const IntPtr>& offset,
                         const Val<const SizeiPtr>& size,
                         const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlBuffer::bindRange>(target, index, offset, size, src_loc)){return;}
    glBindBufferRange(target, index, id(), offset, size);
    debug(src_loc);
}

void GlBuffer::bindUniformBase(const Val<const UInt>& index,
                               const Val<const SrcLoc>& src_loc) const {
    bindBase(GL_UNIFORM_BUFFER, index, src_loc);
}

void GlBuffer::bindUniformRange(const Val<const UInt>& index,
                                const Val<const IntPtr>& offset,
                                const Val<const SizeiPtr>& size,
                                const Val<const SrcLoc>& src_loc) const {
    bindRange(GL_UNIFORM_BUFFER, index, offset, size, src_loc);
}

void GlBuffer::bindShaderStorageBase(const Val<const UInt>& index,
                                     const Val<const SrcLoc>& src_loc) const {
    bindBase(GL_SHADER_STORAGE_BUFFER, index, src_loc);
}

void GlBuffer::bindShaderStorageRange(const Val<const UInt>& index,
                                      const Val<const IntPtr>& offset,
                                      const Val<const SizeiPtr>& size,
                                      const Val<const SrcLoc>& src_loc) const {
    bindRange(GL_SHADER_STORAGE_BUFFER, index, offset, size, src_loc);
}

void GlBuffer::getParameteriv(const Val<const Enum>& pname,
                              const Val<Int[]>& params,
                              const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlBuffer::getParameteriv>(pname, params, src_loc)){return;}
    glGetNamedBufferParameteriv(id(), pname, params);
    debug(src_loc);
}

void GlBuffer::getMapAccess(const Val<Enum>& dst,
                            const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_BUFFER_ACCESS, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlBuffer::getMapRangeAccess(const Val<BitField>& dst,
                                 const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_BUFFER_ACCESS_FLAGS, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlBuffer::isImmutable(const Val<bool>& dst,
                           const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_BUFFER_IMMUTABLE_STORAGE, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlBuffer::isMapped(const Val<bool>& dst,
                        const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_BUFFER_MAPPED, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlBuffer::getSize(const Val<Int>& dst,
                       const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_BUFFER_SIZE, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlBuffer::getStorageFlags(const Val<BitField>& dst,
                               const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_BUFFER_STORAGE_FLAGS, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlBuffer::getUsage(const Val<Enum>& dst,
                        const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_BUFFER_USAGE, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlBuffer::getParameteri64v(const Val<const Enum>& pname,
                                const Val<Int64[]>& params,
                                const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlBuffer::getParameteri64v>(pname, params, src_loc)){return;}
    glGetBufferParameteri64v(id(), pname, params);
    debug(src_loc);
}

void GlBuffer::getMapLength(const Val<Int64>& dst,
                            const Val<const SrcLoc>& src_loc) const {
    getParameteri64v(GL_BUFFER_MAP_LENGTH, dst.cast_reinterpret<Int64[]>(), src_loc);
}

void GlBuffer::getMapOffset(const Val<Int64>& dst,
                            const Val<const SrcLoc>& src_loc) const {
    getParameteri64v(GL_BUFFER_MAP_OFFSET, dst.cast_reinterpret<Int64[]>(), src_loc);
}

void GlBuffer::getSubData(const Val<const IntPtr>& offset,
                          const Val<const SizeiPtr>& size,
                          const Val<void>& data,
                          const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlBuffer::getSubData>(offset, size, data, src_loc)){return;}
    glGetNamedBufferSubData(id(), offset, size, data);
    debug(src_loc);
}

void GlBuffer::setSubData(const Val<const IntPtr>& offset,
                          const Val<const SizeiPtr>& size,
                          const Val<const void>& data,
                          const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlBuffer::setSubData>(offset, size, data, src_loc)){return;}
    glNamedBufferSubData(id(), offset, size, data);
    debug(src_loc);
}

void GlBuffer::copySubData(const Val<const GlBuffer>& readBuffer,
                           const Val<const IntPtr>& read_offset,
                           const Val<const IntPtr>& write_offset,
                           const Val<const SizeiPtr>& size,
                           const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlBuffer::copySubData>(readBuffer, read_offset, write_offset, size, src_loc)){return;}
    glCopyNamedBufferSubData(readBuffer->id(), id(), read_offset, write_offset, size);
    debug(src_loc);
}

void GlBuffer::getPointerv(const Val<void*>& params,
                           const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlBuffer::getPointerv>(params, src_loc)){return;}
    glGetNamedBufferPointerv(id(), GL_BUFFER_MAP_POINTER, params);
    debug(src_loc);
}

void GlBuffer::map(const Val<void*>& dst,
                   const Val<const Enum>& access,
                   const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlBuffer::map>(dst, access, src_loc)){return;}
    *dst = glMapNamedBuffer(id(), access);
    debug(src_loc);
}

void GlBuffer::mapRange(const Val<void*>& dst,
                        const Val<const IntPtr>& offset,
                        const Val<const SizeiPtr>& length,
                        const Val<const BitField>& access, 
                        const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlBuffer::mapRange>(dst, offset, length, access, src_loc)){return;}
    *dst = glMapNamedBufferRange(id(), offset, length, access);
    debug(src_loc);
}

void GlBuffer::flushMappedRange(const Val<const IntPtr>& offset,
                                const Val<const SizeiPtr>& length,
                                const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlBuffer::flushMappedRange>(offset, length, src_loc)){return;}
    glFlushMappedNamedBufferRange(id(), offset, length);
    debug(src_loc);
}

void GlBuffer::unmap(const Val<bool>& dst,
                     const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlBuffer::unmap>(dst, src_loc)){return;}
    *dst = glUnmapNamedBuffer(id());
    debug(src_loc);
}

void GlBuffer::_initer(const Val<UInt>& dst, const Val<const SrcLoc>& src_loc){
    glCreateBuffers(1, dst);
    debug(src_loc);
}

void GlBuffer::_deleter(const UInt& id){
    glDeleteBuffers(1, &id);
}