#include "native/gl/VertexArray.hpp"

#include "glad/gl.h"

using namespace nglpmt::native;

std::shared_ptr<GlVertexArray> GlVertexArray::make(const std::shared_ptr<Context>& ctx,
                                               const Val<const SrcLoc>& src_loc){
    std::shared_ptr<GlVertexArray>(new GlVertexArray(ctx, src_loc));
}

GlVertexArray::GlVertexArray(const std::shared_ptr<Context>& ctx, const Val<const SrcLoc>& src_loc) :
    GlObject(ctx, &GlVertexArray::_initer, &GlVertexArray::_deleter, src_loc){
}

void GlVertexArray::getIndexedParameteriv(const Val<const UInt>& index,
                                          const Val<const Enum>& pname,
                                          const Val<Int[]>& params,
                                          const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlVertexArray::getIndexedParameteriv>(index, pname, params, src_loc)){return;}    
    glGetVertexArrayIndexediv(id(), index, pname, params);
    debug(src_loc);    
}

void GlVertexArray::isAttribEnabled(const Val<const UInt>& index, 
                                    const Val<bool>& dst,
                                    const Val<const SrcLoc>& src_loc) const {
    getIndexedParameteriv(index, GL_VERTEX_ATTRIB_ARRAY_ENABLED, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlVertexArray::getAttribSize(const Val<const UInt>& index, 
                                  const Val<Int>& dst,
                                  const Val<const SrcLoc>& src_loc) const {
    getIndexedParameteriv(index, GL_VERTEX_ATTRIB_ARRAY_SIZE, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlVertexArray::getAttribStride(const Val<const UInt>& index, 
                                    const Val<Int>& dst,
                                    const Val<const SrcLoc>& src_loc) const {
    getIndexedParameteriv(index, GL_VERTEX_ATTRIB_ARRAY_STRIDE, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlVertexArray::getAttribType(const Val<const UInt>& index, 
                                  const Val<Enum>& dst,
                                  const Val<const SrcLoc>& src_loc) const {
    getIndexedParameteriv(index, GL_VERTEX_ATTRIB_ARRAY_TYPE, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlVertexArray::isAttribNormalized(const Val<const UInt>& index, 
                                       const Val<bool>& dst,
                                       const Val<const SrcLoc>& src_loc) const {
    getIndexedParameteriv(index, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlVertexArray::isAttribInteger(const Val<const UInt>& index, 
                                    const Val<bool>& dst,
                                    const Val<const SrcLoc>& src_loc) const {
    getIndexedParameteriv(index, GL_VERTEX_ATTRIB_ARRAY_INTEGER, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlVertexArray::isAttribLong(const Val<const UInt>& index, 
                                 const Val<bool>& dst,
                                 const Val<const SrcLoc>& src_loc) const {
    getIndexedParameteriv(index, GL_VERTEX_ATTRIB_ARRAY_LONG, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlVertexArray::getAttribDivisor(const Val<const UInt>& index, 
                                     const Val<Int>& dst,
                                     const Val<const SrcLoc>& src_loc) const {
    getIndexedParameteriv(index, GL_VERTEX_ATTRIB_ARRAY_DIVISOR, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlVertexArray::getAttribRelativeOffset(const Val<const UInt>& index, 
                                            const Val<Int>& dst,
                                            const Val<const SrcLoc>& src_loc) const {
    getIndexedParameteriv(index, GL_VERTEX_ATTRIB_RELATIVE_OFFSET, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlVertexArray::getIndexedParameteri64v(const Val<const UInt>& index,
                                            const Val<const Enum>& pname,
                                            const Val<Int64[]>& params,
                                            const Val<const SrcLoc>& src_loc ) const {
    if (movedToContext<&GlVertexArray::getIndexedParameteri64v>(index, pname, params, src_loc)){return;}
    glGetVertexArrayIndexed64iv(id(), index, pname, params);
    debug(src_loc);
}

void GlVertexArray::getAttribBindingOffset(const Val<Int64>& dst, const Val<const UInt>& index,
                                         const Val<const SrcLoc>& src_loc) const {
    getIndexedParameteri64v(index, GL_VERTEX_BINDING_OFFSET, dst.cast_reinterpret<Int64[]>(), src_loc);
}

void GlVertexArray::enableAttrib(const Val<const UInt>& index, 
                                 const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlVertexArray::enableAttrib>(index, src_loc)){return;}
    glEnableVertexArrayAttrib(id(), index);
    debug(src_loc);
}

void GlVertexArray::disableAttrib(const Val<const UInt>& index, 
                                  const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlVertexArray::disableAttrib>(index, src_loc)){return;}
    glDisableVertexArrayAttrib(id(), index);
    debug(src_loc);
}

void GlVertexArray::setAttribBinding(const Val<const UInt>& attribindex,
                                     const Val<const UInt>& bindingindex, 
                                     const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlVertexArray::setAttribBinding>(attribindex, bindingindex, src_loc)){return;}
    glVertexArrayAttribBinding(id(), attribindex, bindingindex);
    debug(src_loc);
}

void GlVertexArray::setAttribFormat(const Val<const UInt>& attribindex,
                                    const Val<const Int>& size,
                                    const Val<const Enum>& type,
                                    const Val<const bool>& normalized,
                                    const Val<const UInt>& relativeOffset, 
                                    const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlVertexArray::setAttribFormat>(attribindex, size, type, normalized, relativeOffset, src_loc)){return;}
    glVertexArrayAttribFormat(id(), attribindex, size, type, normalized, relativeOffset);
    debug(src_loc);
}

void GlVertexArray::setBindingDivisor(const Val<const UInt>& bindingindex,
                                      const Val<const UInt>& divisor, 
                                      const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlVertexArray::setBindingDivisor>(bindingindex, divisor, src_loc)){return;}
    glVertexArrayBindingDivisor(id(), bindingindex, divisor);
    debug(src_loc);
}

void GlVertexArray::setElementBuffer(const Val<const GlBuffer>& buffer, 
                                     const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlVertexArray::setElementBuffer>(buffer, src_loc)){return;}
    glVertexArrayElementBuffer(id(), buffer->id());
    debug(src_loc);
}

void GlVertexArray::setVertexBuffer(const Val<const UInt>& bindingindex,
                                    const Val<const GlBuffer>& buffer,
                                    const Val<const IntPtr>& offset,
                                    const Val<const Sizei>& stride, 
                                    const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlVertexArray::setVertexBuffer>(bindingindex, buffer, offset, stride, src_loc)){return;}
    glVertexArrayVertexBuffer(id(), bindingindex, buffer->id(), offset, stride);
    debug(src_loc);
}

void GlVertexArray::drawInstanced(const Val<const Enum>& mode,
                                  const Val<const Sizei>& count,
                                  const Val<const Enum>& type,
                                  const Val<const Sizei>& instances, 
                                  const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlVertexArray::drawInstanced>(mode, count, type, instances, src_loc)){return;}
    glBindVertexArray(id());
    glDrawElementsInstanced(mode, count, type, nullptr, instances);
    glBindVertexArray(0);
    debug(src_loc);
}

void GlVertexArray::_initer(const Val<UInt>& dst, const Val<const SrcLoc>& src_loc){
    glCreateVertexArrays(1, dst);
}

void GlVertexArray::_deleter(const UInt& id){
    glDeleteVertexArrays(1, &id);
}