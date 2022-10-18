#pragma once

#include "native/gl/Object.hpp"

namespace nglpmt::native {

class GlBuffer : public GlObject<GlBuffer> {
public:
    static std::shared_ptr<GlBuffer> make(const std::shared_ptr<Context>& ctx,
                                          const Val<const SrcLoc>& src_loc = SrcLoc{});
    virtual ~GlBuffer();
    
    // glNamedBufferData
    void data(const Val<const SizeiPtr>& size,
              const Val<const void>& data,
              const Val<const Enum>& usage,
              const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glNamedBufferStorage
    void storage(const Val<const SizeiPtr>& size,
                 const Val<const void>& data,
                 const Val<const BitField>& flags,
                 const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glBindBufferBase
    void bindBase(const Val<const Enum>& target,
                  const Val<const UInt>& index,
                  const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void bindUniformBase(const Val<const UInt>& index,
                         const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void bindShaderStorageBase(const Val<const UInt>& index,
                               const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glBindBufferRange
    void bindRange(const Val<const Enum>& target,
                   const Val<const UInt>& index,
                   const Val<const IntPtr>& offset,
                   const Val<const SizeiPtr>& size,
                   const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void bindUniformRange(const Val<const UInt>& index,
                          const Val<const IntPtr>& offset,
                          const Val<const SizeiPtr>& size,
                          const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void bindShaderStorageRange(const Val<const UInt>& index,
                                const Val<const IntPtr>& offset,
                                const Val<const SizeiPtr>& size,
                                const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glGetBufferParameteriv
    void getParameteriv(const Val<const Enum>& pname,
                        const Val<Int[]>& params,
                        const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getMapAccess(const Val<Enum>& dst,
                      const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getMapRangeAccess(const Val<BitField>& dst,
                           const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void isImmutable(const Val<bool>& dst,
                     const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void isMapped(const Val<bool>& dst,
                  const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getSize(const Val<Int>& dst,
                 const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getStorageFlags(const Val<BitField>& dst,
                         const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getUsage(const Val<Enum>& dst,
                  const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glGetBufferParameteri64v
    void getParameteri64v(const Val<const Enum>& pname,
                          const Val<Int64[]>& params,
                          const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getMapLength(const Val<Int64>& dst,
                      const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getMapOffset(const Val<Int64>& dst,
                      const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glGetNamedBufferSubData
    void getSubData(const Val<const IntPtr>& offset,
                    const Val<const SizeiPtr>& size,
                    const Val<void>& data,
                    const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glNamedBufferSubData
    void setSubData(const Val<const IntPtr>& offset,
                    const Val<const SizeiPtr>& size,
                    const Val<const void>& data,
                    const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glCopyNamedBufferSubData
    void copySubData(const Val<const GlBuffer>& readBuffer,
                     const Val<const IntPtr>& readOffset,
                     const Val<const IntPtr>& writeOffset,
                     const Val<const SizeiPtr>& size,
                     const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glGetNamedBufferPointerv
    void getPointerv(const Val<void*>& params,
                     const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glMapNamedBuffer
    void map(const Val<void*>& dst,
             const Val<const Enum>& access,
             const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glMapNamedBufferRange
    void mapRange(const Val<void*>& dst,
                  const Val<const IntPtr>& offset,
                  const Val<const SizeiPtr>& length,
                  const Val<const BitField>& access, 
                  const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glFlushMappedNamedBufferRange
    void flushMappedRange(const Val<const IntPtr>& offset,
                          const Val<const SizeiPtr>& length,
                          const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glUnmapNamedBuffer
    void unmap(const Val<bool>& dst,
               const Val<const SrcLoc>& src_loc = SrcLoc{});

protected:
    GlBuffer(const std::shared_ptr<Context>& ctx, const Val<const SrcLoc>& src_loc);

private:
    static void _initer(const Val<UInt>& dst,
                        const Val<const SrcLoc>& src_loc);
                        
    static void _deleter(const UInt& dst);
};

} // namespace nglpmt::native