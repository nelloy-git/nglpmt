#pragma once

#include "native/gl/Object.hpp"
#include "native/gl/Buffer.hpp"

namespace nglpmt::native {

class GlVertexArray : public GlObject<GlVertexArray> {
public:
    static std::shared_ptr<GlVertexArray> make(const std::shared_ptr<Context>& ctx,
                                             const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glGetVertexArrayIndexediv
    void getIndexedParameteriv(const Val<const UInt>& index,
                               const Val<const Enum>& pname,
                               const Val<Int[]>& params,
                               const Val<const SrcLoc>& src_loc = SrcLoc{}) const;
    
    void isAttribEnabled(const Val<const UInt>& index, 
                         const Val<bool>& dst,
                         const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getAttribSize(const Val<const UInt>& index, 
                       const Val<Int>& dst,
                       const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getAttribStride(const Val<const UInt>& index, 
                         const Val<Int>& dst,
                         const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getAttribType(const Val<const UInt>& index, 
                       const Val<Enum>& dst,
                       const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void isAttribNormalized(const Val<const UInt>& index, 
                            const Val<bool>& dst,
                            const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void isAttribInteger(const Val<const UInt>& index, 
                         const Val<bool>& dst,
                         const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void isAttribLong(const Val<const UInt>& index, 
                      const Val<bool>& dst,
                      const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getAttribDivisor(const Val<const UInt>& index, 
                          const Val<Int>& dst,
                          const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getAttribRelativeOffset(const Val<const UInt>& index, 
                                 const Val<Int>& dst,
                                 const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glGetVertexArrayIndexed64iv
    void getIndexedParameteri64v(const Val<const UInt>& index,
                                 const Val<const Enum>& pname,
                                 const Val<Int64[]>& params,
                                 const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getAttribBindingOffset(const Val<Int64>& dst, const Val<const UInt>& index, 
                                const Val<const SrcLoc>& src_loc = SrcLoc{}) const;
    
    // glEnableVertexArrayAttrib
    void enableAttrib(const Val<const UInt>& index, 
                      const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glDisableVertexArrayAttrib
    void disableAttrib(const Val<const UInt>& index, 
                       const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glVertexArrayAttribBinding
    void setAttribBinding(const Val<const UInt>& attribindex, 
                          const Val<const UInt>& bindingindex, 
                          const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glVertexArrayAttribFormat
    void setAttribFormat(const Val<const UInt>& attribindex,
                         const Val<const Int>& size,
                         const Val<const Enum>& type,
                         const Val<const bool>& normalized,
                         const Val<const UInt>& relativeOffset, 
                         const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glVertexArrayBindingDivisor
    void setBindingDivisor(const Val<const UInt>& bindingindex,
                           const Val<const UInt>& divisor, 
                           const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glVertexArrayElementBuffer
    void setElementBuffer(const Val<const GlBuffer>& buffer, 
                          const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glVertexArrayVertexBuffer
    void setVertexBuffer(const Val<const UInt>& bindingindex,
                         const Val<const GlBuffer>& buffer,
                         const Val<const IntPtr>& offset,
                         const Val<const Sizei>& stride, 
                         const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glDrawElementsInstanced
    void drawInstanced(const Val<const Enum>& mode,
                       const Val<const Sizei>& count,
                       const Val<const Enum>& type,
                       const Val<const Sizei>& instances, 
                       const Val<const SrcLoc>& src_loc = SrcLoc{}) const;
protected:
    GlVertexArray(const std::shared_ptr<Context>& ctx,
                const Val<const SrcLoc>& src_loc = SrcLoc{});

private:
    static void _initer(const Val<UInt>& dst,
                        const Val<const SrcLoc>& src_loc);

    static void _deleter(const UInt& id);
};
    
} // namespace glwpp::gl
