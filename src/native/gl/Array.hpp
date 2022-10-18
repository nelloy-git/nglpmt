#pragma once

#include "native/gl/Buffer.hpp"

namespace nglpmt::native {

template<typename T>
class GlArray : public GlBuffer {
public:
    static sptr<GlArray> make(const sptr<Context>& ctx,
                              const Val<const UInt>& size,
                              const Val<const T>& initial,
                              const Val<const utils::SrcLoc>& src_loc = utils::SrcLoc{}){
        auto self = std::shared_ptr<GlArray>(new GlArray(ctx, size, initial, src_loc));
        static const Val<T*> empty(nullptr);
        self->storage(self->_size * sizeof(T), empty, _access, src_loc);
        for (UInt i = 0; i < self->_size; ++i){
            self->set(i, initial, src_loc);
        }
        return self;
    };

    ~GlArray(){};

    void size(const Val<UInt>& dst,
              const Val<const utils::SrcLoc>& src_loc = utils::SrcLoc{}) const {
        if (movedToContext<&GlArray::size>(dst, src_loc)){return;};
        *dst = _size;
    }

    void get(const Val<T>& dst,
             const Val<const UInt>& i,
             const Val<const utils::SrcLoc>& src_loc = utils::SrcLoc{}) const {
        if (movedToContext<&GlArray::get>(dst, i, src_loc)){return;}
        Val<T*> mapped;
        mapRange(mapped, *i * sizeof(T), sizeof(T), _access, src_loc);
        *dst = *mapped;
        unmap(true, src_loc);
    }

    bool set(const Val<const T>& value,
             const Val<const UInt>& i, 
             const Val<const utils::SrcLoc>& src_loc = utils::SrcLoc{}){
        if (movedToContext<&GlArray::set>(value, i, src_loc)){return;}
        Val<void*> mapped(nullptr);
        mapRange(mapped, *i * sizeof(T), sizeof(T), _access, src_loc);
        auto mapped_struct = mapped.cast_reinterpret<T*>();
        **mapped_struct = *value;
        unmap(true, src_loc);
    }

protected:
    Array(const sptr<Context>& ctx,
          const Val<const UInt>& size,
          const Val<const T>& initial,
          const Val<const utils::SrcLoc>& src_loc) :
        Buffer(ctx, src_loc),
        _size(size){
    }

private:
    // Hide parent's make
    using Buffer::make;

    UInt _size;
    static inline BitField _access = static_cast<Enum>(BufferStorageFlag::Read)
                                   | static_cast<Enum>(BufferStorageFlag::Write)
                                   | static_cast<Enum>(BufferStorageFlag::Persistent)
                                   | static_cast<Enum>(BufferStorageFlag::Coherent);
};

} // namespace nglpmt::native