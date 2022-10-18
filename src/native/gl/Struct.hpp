#pragma once

#include "native/gl/Buffer.hpp"

namespace glwpp::gl {

template<typename T>
class Struct : public GlBuffer {
public:
    static std::shared_ptr<Struct> make(const std::shared_ptr<Context>& ctx,
                                        const Val<const T>& initial,
                                        const Val<const utils::SrcLoc>& src_loc = utils::SrcLoc{}){
        auto self = std::shared_ptr<Struct>(new Struct(ctx, initial, src_loc));
        self->storage(sizeof(T), Val<const void>(initial), _access, src_loc);
        return self;
    };

    ~Struct(){};

    bool get(const Val<T>& dst,
             const Val<const utils::SrcLoc>& src_loc = utils::SrcLoc{}) const {
        if (movedToContext<&Struct::get>(dst, src_loc)){return;}

        Val<T*> mapped;
        map(mapped, BufferMapAccess::ReadOnly, src_loc);
        *dst = *mapped;
        unmap(true, src_loc);
        return true;
    }

    bool set(const Val<const T>& value,
             const Val<const utils::SrcLoc>& src_loc = utils::SrcLoc{}){
        if (movedToContext<&Struct::set>(value, src_loc)){return;}

        Val<void*> mapped(nullptr);
        map(mapped, BufferMapAccess::ReadOnly, src_loc);
        auto mapped_struct = mapped.cast_reinterpret<T*>();
        **mapped_struct = *value;
        unmap(true, src_loc);
        return true;
    }

protected:
    Struct(const std::shared_ptr<Context>& ctx,
           const Val<const T>& initial,
           const Val<const utils::SrcLoc>& src_loc) :
        Buffer(ctx, src_loc){
    }

private:

    static inline const Val<const UInt> _access = static_cast<Enum>(BufferStorageFlag::Read)
                                                  | static_cast<Enum>(BufferStorageFlag::Write);
};

}