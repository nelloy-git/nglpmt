#pragma once

#include <string>

#include "native/gl/Object.hpp"

namespace nglpmt::native {

class GlShader : public GlObject<GlShader> {
public:
    static std::shared_ptr<GlShader> make(const std::shared_ptr<Context>& ctx,
                                          const Val<const Enum>& type,
                                          const Val<const SrcLoc>& src_loc = SrcLoc{});
    
    // glGetShaderiv
    void getParameteriv(const Val<const Enum>& param,
                        const Val<Int[]>& params,
                        const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getType(const Val<Enum>& dst,
                 const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void isCompiled(const Val<bool>& dst,
                    const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getSourceLength(const Val<Int>& dst,
                         const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glGetShaderInfoLog
    void getInfoLog(const Val<std::string>& infoLog,
                    const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glShaderSource
    void source(const Val<const std::string>& code,
                const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glCompileShader
    void compile(const Val<const SrcLoc>& src_loc = SrcLoc{});

protected:
    GlShader(const std::shared_ptr<Context>& ctx,
             const Val<const Enum>& type,
             const Val<const SrcLoc>& src_loc);

private:
    static void _initer(const Val<UInt>& dst,
                        const Val<const Enum>& type,
                        const Val<const SrcLoc>& src_loc);

    static void _deleter(const UInt& id);
};

} // namespace nglpmt::native