#pragma once

#include "native/gl/Object.hpp"
#include "native/gl/Shader.hpp"

namespace nglpmt::native {

class GlProgram : public GlObject<GlProgram> {
public:
    static std::shared_ptr<GlProgram> make(const std::shared_ptr<Context>& ctx,
                                           const Val<const SrcLoc>& src_loc = SrcLoc{});
            
    // glGetProgramiv
    void getParameteriv(const Val<const Enum>& pname,
                        const Val<Int[]>& params,
                        const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void isLinked(const Val<bool>& dst,
                  const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void isValidated(const Val<bool>& dst,
                     const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getAttachedShadersCount(const Val<Int>& dst,
                                 const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getActiveAttributesCount(const Val<Int>& dst,
                                  const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getActiveAttributeMaxNameLength(const Val<Int>& dst,
                                         const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getActiveUniformsCount(const Val<Int>& dst,
                                const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    void getActiveUniformMaxNameLength(const Val<Int>& dst,
                                       const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glGetProgramInfoLog
    void getInfoLog(const Val<std::string>& infoLog,
                    const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glGetAttribLocation
    void getAttributeLocation(const Val<Int>& dst,
                              const Val<const std::string>& name,
                              const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glBindAttribLocation                    
    void bindAttributeLocation(const Val<const UInt>& index,
                               const Val<const std::string>& name,
                               const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glGetUniformLocation
    void getUniformLocation(const Val<Int>& dst,
                            const Val<const std::string>& name,
                            const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glGetUniformBlockIndex
    void getUniformBlockIndex(const Val<UInt>& dst,
                              const Val<const std::string>& name,
                              const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glAttachShader
    void attach(const Val<const GlShader>& shader,
                const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glLinkProgram
    void link(const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glValidateProgram
    void validate(const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glUseProgram
    void use(const Val<const SrcLoc>& src_loc = SrcLoc{}) const;

    // glProgramUniform1f
    void uniform1f(const Val<const Int>& location,
                   const Val<const Float>& v0,
                   const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform2f
    void uniform2f(const Val<const Int>& location,
                   const Val<const Float>& v0,
                   const Val<const Float>& v1,
                   const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform3f
    void uniform3f(const Val<const Int>& location,
                   const Val<const Float>& v0,
                   const Val<const Float>& v1,
                   const Val<const Float>& v2,
                   const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform4f
    void uniform4f(const Val<const Int>& location,
                   const Val<const Float>& v0,
                   const Val<const Float>& v1,
                   const Val<const Float>& v2,
                   const Val<const Float>& v3,
                   const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform1fv
    void uniform1fv(const Val<const Int>& location,
                    const Val<const Sizei>& count,
                    const Val<const Float[]>& data,
                    const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform2fv
    void uniform2fv(const Val<const Int>& location,
                    const Val<const Sizei>& count,
                    const Val<const Float[]>& data,
                    const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform3fv
    void uniform3fv(const Val<const Int>& location,
                    const Val<const Sizei>& count,
                    const Val<const Float[]>& data,
                    const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform4fv
    void uniform4fv(const Val<const Int>& location,
                    const Val<const Sizei>& count,
                    const Val<const Float[]>& data,
                    const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniformMatrix2fv
    void uniformMatrix2fv(const Val<const Int>& location,
                          const Val<const Sizei>& count,
                          const Val<const bool>& transpose,
                          const Val<const Float[]>& data,
                          const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniformMatrix2x3fv
    void uniformMatrix2x3fv(const Val<const Int>& location,
                            const Val<const Sizei>& count,
                            const Val<const bool>& transpose,
                            const Val<const Float[]>& data,
                            const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniformMatrix2x4fv
    void uniformMatrix2x4fv(const Val<const Int>& location,
                            const Val<const Sizei>& count,
                            const Val<const bool>& transpose,
                            const Val<const Float[]>& data,
                            const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniformMatrix3fv
    void uniformMatrix3fv(const Val<const Int>& location,
                          const Val<const Sizei>& count,
                          const Val<const bool>& transpose,
                          const Val<const Float[]>& data,
                          const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniformMatrix3x2fv
    void uniformMatrix3x2fv(const Val<const Int>& location,
                            const Val<const Sizei>& count,
                            const Val<const bool>& transpose,
                            const Val<const Float[]>& data,
                            const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniformMatrix3x4fv
    void uniformMatrix3x4fv(const Val<const Int>& location,
                            const Val<const Sizei>& count,
                            const Val<const bool>& transpose,
                            const Val<const Float[]>& data,
                            const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniformMatrix4fv
    void uniformMatrix4fv(const Val<const Int>& location,
                          const Val<const Sizei>& count,
                          const Val<const bool>& transpose,
                          const Val<const Float[]>& data,
                          const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniformMatrix4x2fv
    void uniformMatrix4x2fv(const Val<const Int>& location,
                            const Val<const Sizei>& count,
                            const Val<const bool>& transpose,
                            const Val<const Float[]>& data,
                            const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniformMatrix4x3fv
    void uniformMatrix4x3fv(const Val<const Int>& location,
                            const Val<const Sizei>& count,
                            const Val<const bool>& transpose,
                            const Val<const Float[]>& data,
                            const Val<const SrcLoc>& src_loc = SrcLoc{});
    
    // glProgramUniform1d
    void uniform1d(const Val<const Int>& location,
                   const Val<const Double>& v0,
                   const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform2d
    void uniform2d(const Val<const Int>& location,
                   const Val<const Double>& v0,
                   const Val<const Double>& v1,
                   const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform3d
    void uniform3d(const Val<const Int>& location,
                   const Val<const Double>& v0,
                   const Val<const Double>& v1,
                   const Val<const Double>& v2,
                   const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform4d
    void uniform4d(const Val<const Int>& location,
                   const Val<const Double>& v0,
                   const Val<const Double>& v1,
                   const Val<const Double>& v2,
                   const Val<const Double>& v3,
                   const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform1dv
    void uniform1dv(const Val<const Int>& location,
                    const Val<const Sizei>& count,
                    const Val<const Double[]>& data,
                    const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform2dv
    void uniform2dv(const Val<const Int>& location,
                    const Val<const Sizei>& count,
                    const Val<const Double[]>& data,
                    const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform3dv
    void uniform3dv(const Val<const Int>& location,
                    const Val<const Sizei>& count,
                    const Val<const Double[]>& data,
                    const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform4dv
    void uniform4dv(const Val<const Int>& location,
                    const Val<const Sizei>& count,
                    const Val<const Double[]>& data,
                    const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniformMatrix2dv
    void uniformMatrix2dv(const Val<const Int>& location,
                          const Val<const Sizei>& count,
                          const Val<const bool>& transpose,
                          const Val<const Double[]>& data,
                          const Val<const SrcLoc>& src_loc = SrcLoc{});
 
    // glProgramUniformMatrix2x3dv
    void uniformMatrix2x3dv(const Val<const Int>& location,
                            const Val<const Sizei>& count,
                            const Val<const bool>& transpose,
                            const Val<const Double[]>& data,
                            const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniformMatrix2x4dv
    void uniformMatrix2x4dv(const Val<const Int>& location,
                            const Val<const Sizei>& count,
                            const Val<const bool>& transpose,
                            const Val<const Double[]>& data,
                            const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniformMatrix3dv
    void uniformMatrix3dv(const Val<const Int>& location,
                          const Val<const Sizei>& count,
                          const Val<const bool>& transpose,
                          const Val<const Double[]>& data,
                          const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniformMatrix3x2dv
    void uniformMatrix3x2dv(const Val<const Int>& location,
                            const Val<const Sizei>& count,
                            const Val<const bool>& transpose,
                            const Val<const Double[]>& data,
                            const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniformMatrix3x4dv
    void uniformMatrix3x4dv(const Val<const Int>& location,
                            const Val<const Sizei>& count,
                            const Val<const bool>& transpose,
                            const Val<const Double[]>& data,
                            const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniformMatrix4dv
    void uniformMatrix4dv(const Val<const Int>& location,
                          const Val<const Sizei>& count,
                          const Val<const bool>& transpose,
                          const Val<const Double[]>& data,
                          const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniformMatrix4x2dv
    void uniformMatrix4x2dv(const Val<const Int>& location,
                            const Val<const Sizei>& count,
                            const Val<const bool>& transpose,
                            const Val<const Double[]>& data,
                            const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniformMatrix4x3dv
    void uniformMatrix4x3dv(const Val<const Int>& location,
                            const Val<const Sizei>& count,
                            const Val<const bool>& transpose,
                            const Val<const Double[]>& data,
                            const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform1i
    void uniform1i(const Val<const Int>& location,
                   const Val<const Int>& v0,
                   const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform2i
    void uniform2i(const Val<const Int>& location,
                   const Val<const Int>& v0,
                   const Val<const Int>& v1,
                   const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform3i
    void uniform3i(const Val<const Int>& location,
                   const Val<const Int>& v0,
                   const Val<const Int>& v1,
                   const Val<const Int>& v2,
                   const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform4i
    void uniform4i(const Val<const Int>& location,
                   const Val<const Int>& v0,
                   const Val<const Int>& v1,
                   const Val<const Int>& v2,
                   const Val<const Int>& v3,
                   const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform1iv
    void uniform1iv(const Val<const Int>& location,
                    const Val<const Sizei>& count,
                    const Val<const Int[]>& data,
                    const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform2iv
    void uniform2iv(const Val<const Int>& location,
                    const Val<const Sizei>& count,
                    const Val<const Int[]>& data,
                    const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform3iv
    void uniform3iv(const Val<const Int>& location,
                    const Val<const Sizei>& count,
                    const Val<const Int[]>& data,
                    const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform4iv
    void uniform4iv(const Val<const Int>& location,
                    const Val<const Sizei>& count,
                    const Val<const Int[]>& data,
                    const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform1ui
    void uniform1ui(const Val<const Int>& location,
                    const Val<const UInt>& v0,
                    const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform2ui
    void uniform2ui(const Val<const Int>& location,
                    const Val<const UInt>& v0,
                    const Val<const UInt>& v1,
                    const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform3ui
    void uniform3ui(const Val<const Int>& location,
                    const Val<const UInt>& v0,
                    const Val<const UInt>& v1,
                    const Val<const UInt>& v2,
                    const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform4ui
    void uniform4ui(const Val<const Int>& location,
                    const Val<const UInt>& v0,
                    const Val<const UInt>& v1,
                    const Val<const UInt>& v2,
                    const Val<const UInt>& v3,
                    const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform1uiv
    void uniform1uiv(const Val<const Int>& location,
                     const Val<const Sizei>& count,
                     const Val<const UInt[]>& data,
                     const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform2uiv
    void uniform2uiv(const Val<const Int>& location,
                     const Val<const Sizei>& count,
                     const Val<const UInt[]>& data,
                     const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform3uiv
    void uniform3uiv(const Val<const Int>& location,
                     const Val<const Sizei>& count,
                     const Val<const UInt[]>& data,
                     const Val<const SrcLoc>& src_loc = SrcLoc{});

    // glProgramUniform4uiv
    void uniform4uiv(const Val<const Int>& location,
                     const Val<const Sizei>& count,
                     const Val<const UInt[]>& data,
                     const Val<const SrcLoc>& src_loc = SrcLoc{});

protected:
    GlProgram(const std::shared_ptr<Context>& ctx,
            const Val<const SrcLoc>& src_loc);

private:
    static void _initer(const Val<UInt>& dst,
                        const Val<const SrcLoc>& src_loc);
    static void _deleter(const UInt& id);
};

} // namespace glwpp