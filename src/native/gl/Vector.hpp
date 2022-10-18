#pragma once

#include <optional>

#include "glwpp/gl/enums/BufferStorageFlag.hpp"
#include "glwpp/gl/obj/Buffer.hpp"

namespace glwpp::gl {

template<typename T>
class Vector : public Buffer {
public:
    static sptr<Vector> make1(const sptr<Context>& ctx,
                             const Val<const UInt>& size,
                             const Val<const BufferUsage>& usage,
                             const Val<const T>& initial,
                             const Val<const utils::SrcLoc>& src_loc = utils::SrcLoc{}){
        auto self = sptr<Vector>(new Vector(ctx, size, usage, initial, src_loc));
        static const Val<T*> empty(nullptr);
        self->data(self->_capacity * sizeof(T), empty, self->_usage);
        for (UInt i = 0; i < self->_size; ++i){
            self->set(i, initial, src_loc);
        }
        return self;
    }

    ~Vector(){};

    bool getSize(const Val<UInt>& dst,
                 const Val<const utils::SrcLoc>& src_loc = utils::SrcLoc{}) const {
        if (!isContextThread()){
            return executeMethodInContext(&Vector::getSize, dst, src_loc);
        }

        *dst = _size;
        debug(src_loc);
        return true;
    }

    bool getCapacity(const Val<UInt>& dst,
                     const Val<const utils::SrcLoc>& src_loc = utils::SrcLoc{}) const {
        if (!isContextThread()){
            return executeMethodInContext(&Vector::getCapacity, dst, src_loc);
        }

        *dst = _capacity;
        debug(src_loc);
        return true;
    }

    bool get(const Val<const UInt>& i, const Val<T>& dst,
             const Val<const utils::SrcLoc>& src_loc = utils::SrcLoc{}) const {
        if (!isContextThread()){
            return executeMethodInContext(&Vector::get, i, dst, src_loc);
        }

        if (*i >= _size){
            throw std::out_of_range("glwpp::gl::Vector");
        }

        getSubData(dst, *i * sizeof(T), sizeof(T), src_loc);
        debug(src_loc);
        return true;
    }

    bool set(const Val<const UInt>& i, const Val<const T>& value,
             const Val<const utils::SrcLoc>& src_loc = utils::SrcLoc{}){
        if (!isContextThread()){
            return executeMethodInContext(&Vector::set, i, value, src_loc);
        }

        if (*i >= _size){
            throw std::out_of_range("glwpp::gl::Vector");
        }

        setSubData(value.cast_reinterpret<const void>(), *i * sizeof(T), sizeof(T), src_loc);
        debug(src_loc);
        return true;
    }

    bool reserve(const Val<const UInt>& capacity,
                 const Val<const utils::SrcLoc>& src_loc = utils::SrcLoc{}){
        if (!isContextThread()){
            return executeMethodInContext(&Vector::reserve, capacity, src_loc);
        }

        if (*capacity <= _capacity){
            return true;
        }

        auto tmp = Buffer::make(getContext().lock(), src_loc);
        tmp->data(_size * sizeof(T), Val<const void>(sptr<void>()), BufferUsage::StreamCopy, src_loc);
        tmp->copySubData(std::static_pointer_cast<const Vector>(shared_from_this()), 0, 0, _size * sizeof(T), src_loc);

        _capacity = *capacity;
        data(_capacity * sizeof(T), Val<const void>(sptr<void>()), _usage, src_loc);
        copySubData(tmp, 0, 0, _size * sizeof(T), src_loc);

        debug(src_loc);
        return true;
    }

    bool shape(const Val<const utils::SrcLoc>& src_loc = utils::SrcLoc{}){
        if (!isContextThread()){
            return executeMethodInContext(&Vector::shape, src_loc);
        }

        auto tmp = Buffer::make(getContext().lock(), src_loc);
        tmp->data(_size * sizeof(T), Val<const void>(sptr<void>()), BufferUsage::StreamCopy, src_loc);
        tmp->copySubData(shared_from_this(), 0, 0, _size * sizeof(T), src_loc);

        _capacity = _size;
        data(_size * sizeof(T), Val<const void>(sptr<void>()), _usage, src_loc);
        copySubData(tmp, 0, 0, _size * sizeof(T), src_loc);

        debug(src_loc);
        return true;
    }

    bool pushBack(const Val<const std::optional<T>>& value,
                  const Val<const utils::SrcLoc>& src_loc = utils::SrcLoc{}){
        if (!isContextThread()){
            return executeMethodInContext(&Vector::pushBack, value, src_loc);
        }

        if (_size == _capacity){
            reserve(2 * *_size, src_loc);
        }

        set(_size, value, src_loc);
        ++_size;
        debug(src_loc);
        return true;
    }

    bool popBack(const Val<T>& dst,
                 const Val<const utils::SrcLoc>& src_loc = utils::SrcLoc{}){
        if (!isContextThread()){
            return executeMethodInContext(&Vector::popBack, dst, src_loc);
        }

        if (_size == 0){
            throw std::out_of_range("glwpp::gl::Vector");
        }
        
        get(*_size - 1, dst, src_loc);
        --_size;
        debug(src_loc);
        return true;
    }

protected:
    Vector(const sptr<Context>& ctx,
           const Val<const UInt>& size,
           const Val<const BufferUsage>& usage,
           const Val<const T>& initial,
           const Val<const utils::SrcLoc>& src_loc = utils::SrcLoc{}) :
        Buffer(ctx, src_loc),
        _size(size),
        _capacity(*size < 4 ? 4 : *size),
        _usage(usage){
    }

private:
    // Hide parent's make
    using Buffer::make;

    UInt _size;
    UInt _capacity;
    BufferUsage _usage;
};

}