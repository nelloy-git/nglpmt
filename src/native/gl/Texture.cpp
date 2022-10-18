#include "native/gl/Texture.hpp"

#include "glad/gl.h"

using namespace nglpmt::native;

std::shared_ptr<GlTexture> GlTexture::make(const std::shared_ptr<Context>& ctx,
                                       const Val<const Enum>& type,
                                       const Val<const SrcLoc>& src_loc){
    std::shared_ptr<GlTexture>(new GlTexture(ctx, type, src_loc));
}

GlTexture::GlTexture(const std::shared_ptr<Context>& ctx,
                     const Val<const Enum>& type,
                     const Val<const SrcLoc>& src_loc) :
    GlObject(ctx, &GlTexture::_initer, &GlTexture::_deleter, type, src_loc){
}

void GlTexture::buffer(const Val<const Enum>& internalformat,
                       const Val<const GlBuffer>& buffer,
                       const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlTexture::buffer>(internalformat, buffer, src_loc)){return;}
    glTextureBuffer(id(), internalformat, buffer->id());
    debug(src_loc);
}

void GlTexture::bufferRange(const Val<const Enum>& internalformat,
                            const Val<const GlBuffer>& buffer,
                            const Val<const IntPtr>& offset,
                            const Val<const SizeiPtr>& size,
                            const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlTexture::bufferRange>(internalformat, buffer, offset, size, src_loc)){return;}
    glTextureBufferRange(id(), internalformat, buffer->id(), offset, size);
    debug(src_loc);
}

void GlTexture::storage1D(const Val<const Sizei>& levels,
                          const Val<const Enum>& internalformat,
                          const Val<const Sizei>& width,
                          const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlTexture::storage1D>(levels, internalformat, width, src_loc)){return;}
    glTextureStorage1D(id(), levels, internalformat, width);
    debug(src_loc);
}

void GlTexture::storage2D(const Val<const Sizei>& levels,
                          const Val<const Enum>& internalformat,
                          const Val<const Sizei>& width,
                          const Val<const Sizei>& height,
                          const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlTexture::storage2D>(levels, internalformat, width, height, src_loc)){return;}
    glTextureStorage2D(id(), levels, internalformat, width, height);
    debug(src_loc);
}

void GlTexture::storage3D(const Val<const Sizei>& levels,
                          const Val<const Enum>& internalformat,
                          const Val<const Sizei>& width,
                          const Val<const Sizei>& height,
                          const Val<const Sizei>& depth,
                          const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlTexture::storage3D>(levels, internalformat, width, height, depth, src_loc)){return;}
    glTextureStorage3D(id(), levels, internalformat, width, height, depth);
    debug(src_loc);
}

void GlTexture::storageMultisample2D(const Val<const Sizei>& samples,
                                     const Val<const Enum>& internalformat,
                                     const Val<const Sizei>& width,
                                     const Val<const Sizei>& height,
                                     const Val<const Bool>& fixedsamplelocations,
                                     const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlTexture::storageMultisample2D>(samples, internalformat, width, height, fixedsamplelocations, src_loc)){return;}
    glTextureStorage2DMultisample(id(), samples, internalformat, width, height, fixedsamplelocations);
    debug(src_loc);
}

void GlTexture::storageMultisample3D(const Val<const Sizei>& samples,
                                     const Val<const Enum>& internalformat,
                                     const Val<const Sizei>& width,
                                     const Val<const Sizei>& height,
                                     const Val<const Sizei>& depth,
                                     const Val<const Bool>& fixedsamplelocations,
                                     const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlTexture::storageMultisample3D>(samples, internalformat, width, height, depth, fixedsamplelocations, src_loc)){return;}
    glTextureStorage3DMultisample(id(), samples, internalformat, width, height, depth, fixedsamplelocations);
    debug(src_loc);
}

void GlTexture::bindUnit(const Val<const UInt>& unit,
                         const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlTexture::bindUnit>(unit, src_loc)){return;}
    glBindTextureUnit(unit, id());
    debug(src_loc);
}

void GlTexture::generateMipMap(const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlTexture::generateMipMap>(src_loc)){return;}
    glGenerateTextureMipmap(id());
    debug(src_loc);
}

void GlTexture::getImage(const Val<const Int>& level,
                         const Val<const Enum>& format,
                         const Val<const Enum>& type,
                         const Val<const Sizei>& bufSize,
                         const Val<void>& pixels,
                         const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlTexture::getImage>(level, format, type, bufSize, pixels, src_loc)){return;}
    glGetTextureImage(id(), level, format, type, bufSize, pixels);
    debug(src_loc);
}

void GlTexture::getSubImage(const Val<const Int>& level,
                            const Val<const Int>& xoffset,
                            const Val<const Int>& yoffset,
                            const Val<const Int>& zoffset,
                            const Val<const Sizei>& width,
                            const Val<const Sizei>& height,
                            const Val<const Sizei>& depth,
                            const Val<const Enum>& format,
                            const Val<const Enum>& type,
                            const Val<const Sizei>& bufSize,
                            const Val<void>& pixels, 
                            const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlTexture::getSubImage>(level, xoffset, yoffset, zoffset, width, height, depth, format, type, bufSize, pixels, src_loc)){return;}
    glGetTextureSubImage(id(), level, xoffset, yoffset, zoffset, width, height, depth, format, type, bufSize, pixels);
    debug(src_loc);
}

void GlTexture::setSubImage1D(const Val<const Int>& level,
                              const Val<const Int>& xoffset,
                              const Val<const Sizei>& width,
                              const Val<const Enum>& format,
                              const Val<const Enum>& type,
                              const Val<const void>& pixels,
                              const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlTexture::setSubImage1D>(level, xoffset, width, format, type, pixels, src_loc)){return;}
    glTextureSubImage1D(id(), level, xoffset, width, format, type, pixels);
    debug(src_loc);
}

void GlTexture::setSubImage2D(const Val<const Int>& level,
                              const Val<const Int>& xoffset,
                              const Val<const Int>& yoffset,
                              const Val<const Sizei>& width,
                              const Val<const Sizei>& height,
                              const Val<const Enum>& format,
                              const Val<const Enum>& type,
                              const Val<const void>& pixels,
                              const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlTexture::setSubImage2D>(level, xoffset, yoffset, width, height, format, type, pixels, src_loc)){return;}
    glTextureSubImage2D(id(), level, xoffset, yoffset, width, height, format, type, pixels);
    debug(src_loc);
}

void GlTexture::setSubImage3D(const Val<const Int>& level,
                              const Val<const Int>& xoffset,
                              const Val<const Int>& yoffset,
                              const Val<const Int>& zoffset,
                              const Val<const Sizei>& width,
                              const Val<const Sizei>& height,
                              const Val<const Sizei>& depth,
                              const Val<const Enum>& format,
                              const Val<const Enum>& type,
                              const Val<const void>& pixels,
                              const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlTexture::setSubImage3D>(level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels, src_loc)){return;}
    glTextureSubImage3D(id(), level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
    debug(src_loc);
}

void GlTexture::getParameteriv(const Val<const Enum>& pname,
                               const Val<Int[]>& params,
                               const Val<const SrcLoc>& src_loc) const {
    if (movedToContext<&GlTexture::getParameteriv>(pname, params, src_loc)){return;}
    glGetTextureParameteriv(id(), pname, params);
    debug(src_loc);
}

void GlTexture::setParameteri(const Val<const Enum>& pname,
                              const Val<const Int>& param,
                              const Val<const SrcLoc>& src_loc){
    if (movedToContext<&GlTexture::setParameteri>(pname, param, src_loc)){return;}
    glTextureParameteri(id(), pname, param);
    debug(src_loc);
}

void GlTexture::getWrapS(const Val<Enum>& dst,
                         const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_TEXTURE_WRAP_S, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlTexture::setWrapS(const Val<const Enum>& value,
                         const Val<const SrcLoc>& src_loc){
    setParameteri(GL_TEXTURE_WRAP_S, value.cast_reinterpret<const Int>(), src_loc);
}

void GlTexture::getWrapT(const Val<Enum>& dst,
                         const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_TEXTURE_WRAP_T, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlTexture::setWrapT(const Val<const Enum>& value,
                         const Val<const SrcLoc>& src_loc){
    setParameteri(GL_TEXTURE_WRAP_T, value.cast_reinterpret<const Int>(), src_loc);
}

void GlTexture::getMinFilter(const Val<Enum>& dst,
                             const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_TEXTURE_MIN_FILTER, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlTexture::setMinFilter(const Val<const Enum>& value,
                             const Val<const SrcLoc>& src_loc){
    setParameteri(GL_TEXTURE_MIN_FILTER, value.cast_reinterpret<const Int>(), src_loc);
}

void GlTexture::getMagFilter(const Val<Enum>& dst,
                             const Val<const SrcLoc>& src_loc) const {
    getParameteriv(GL_TEXTURE_MAG_FILTER, dst.cast_reinterpret<Int[]>(), src_loc);
}

void GlTexture::setMagFilter(const Val<const Enum>& value,
                             const Val<const SrcLoc>& src_loc){
    setParameteri(GL_TEXTURE_MAG_FILTER, value.cast_reinterpret<const Int>(), src_loc);
}

void GlTexture::_initer(const Val<UInt>& dst,
                        const Val<const Enum>& type,
                        const Val<const SrcLoc>& src_loc){
    glCreateTextures(type, 1, dst);
    debug(src_loc);
}

void GlTexture::_deleter(const UInt& id){
    glDeleteTextures(1, &id);
}