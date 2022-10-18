#pragma once

#include "native/gl/Object.hpp"
#include "native/gl/Buffer.hpp"

namespace nglpmt::native {

class GlTexture : public GlObject<GlTexture> {
public:
    static std::shared_ptr<GlTexture> make(const std::shared_ptr<Context>& ctx,
                                         const Val<const Enum>& type,
                                         const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glTextureBuffer
    void buffer(const Val<const Enum>& internalformat,
                const Val<const GlBuffer>& buffer,
                const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glTextureBufferRange
    void bufferRange(const Val<const Enum>& internalformat,
                     const Val<const GlBuffer>& buffer,
                     const Val<const IntPtr>& offset,
                     const Val<const SizeiPtr>& size,
                     const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glTextureStorage1D
    void storage1D(const Val<const Sizei>& levels,
                   const Val<const Enum>& internalformat,
                   const Val<const Sizei>& width,
                   const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glTextureStorage2D
    void storage2D(const Val<const Sizei>& levels,
                   const Val<const Enum>& internalformat,
                   const Val<const Sizei>& width,
                   const Val<const Sizei>& height,
                   const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glTextureStorage3D
    void storage3D(const Val<const Sizei>& levels,
                   const Val<const Enum>& internalformat,
                   const Val<const Sizei>& width,
                   const Val<const Sizei>& height,
                   const Val<const Sizei>& depth,
                   const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glTextureStorage2DMultisample
    void storageMultisample2D(const Val<const Sizei>& samples,
                              const Val<const Enum>& internalformat,
                              const Val<const Sizei>& width,
                              const Val<const Sizei>& height,
                              const Val<const Bool>& fixedsamplelocations,
                              const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glTextureStorage3DMultisample
    void storageMultisample3D(const Val<const Sizei>& samples,
                              const Val<const Enum>& fmt,
                              const Val<const Sizei>& width,
                              const Val<const Sizei>& height,
                              const Val<const Sizei>& depth,
                              const Val<const Bool>& fixedsamplelocations,
                              const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glBindTextureUnit
    void bindUnit(const Val<const UInt>& unit,
                 const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glGenerateTextureMipmap
    void generateMipMap(const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glGetTextureImage
    void getImage(const Val<const Int>& level,
                  const Val<const Enum>& format,
                  const Val<const Enum>& type,
                  const Val<const Sizei>& bufSize,
                  const Val<void>& pixels,
                  const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glGetTextureSubImage
    void getSubImage(const Val<const Int>& level,
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
                     const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glTextureSubImage1D
    void setSubImage1D(const Val<const Int>& level,
                       const Val<const Int>& xoffset,
                       const Val<const Sizei>& width,
                       const Val<const Enum>& format,
                       const Val<const Enum>& type,
                       const Val<const void>& pixels,
                       const Val<const SrcLoc>& src_loc = SrcLoc{});
    
    // glTextureSubImage2D
    void setSubImage2D(const Val<const Int>& level,
                       const Val<const Int>& xoffset,
                       const Val<const Int>& yoffset,
                       const Val<const Sizei>& width,
                       const Val<const Sizei>& height,
                       const Val<const Enum>& format,
                       const Val<const Enum>& type,
                       const Val<const void>& pixels,
                       const Val<const SrcLoc>& src_loc = SrcLoc{});
    
    // glTextureSubImage3D
    void setSubImage3D(const Val<const Int>& level,
                       const Val<const Int>& xoffset,
                       const Val<const Int>& yoffset,
                       const Val<const Int>& zoffset,
                       const Val<const Sizei>& width,
                       const Val<const Sizei>& height,
                       const Val<const Sizei>& depth,
                       const Val<const Enum>& format,
                       const Val<const Enum>& type,
                       const Val<const void>& pixels,
                       const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glGetTextureParameteriv
    void getParameteriv(const Val<const Enum>& pname,
                        const Val<Int[]>& params,
                        const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getWrapS(const Val<Enum>& dst,
                  const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getWrapT(const Val<Enum>& dst,
                  const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getMinFilter(const Val<Enum>& dst,
                      const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getMagFilter(const Val<Enum>& dst,
                      const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glTextureParameteri
    void setParameteri(const Val<const Enum>& pname,
                       const Val<const Int>& param,
                       const Val<const SrcLoc>& src_loc = SrcLoc{});

    void setWrapS(const Val<const Enum>& value,
                  const Val<const SrcLoc>& src_loc = SrcLoc{});

    void setWrapT(const Val<const Enum>& value,
                  const Val<const SrcLoc>& src_loc = SrcLoc{});

    void setMinFilter(const Val<const Enum>& value,
                      const Val<const SrcLoc>& src_loc = SrcLoc{});

    void setMagFilter(const Val<const Enum>& value,
                      const Val<const SrcLoc>& src_loc = SrcLoc{});

protected:
    GlTexture(const std::shared_ptr<Context>& ctx,
            const Val<const Enum>& type,
            const Val<const SrcLoc>& src_loc = SrcLoc{});

private:
    static void _initer(const Val<UInt>& dst,
                        const Val<const Enum>& type,
                        const Val<const SrcLoc>& src_loc);
    static void _deleter(const UInt& id);
};

} // namespace glwpp