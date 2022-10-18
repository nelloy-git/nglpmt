#pragma once

#include <memory>

namespace nglpmt::native {

template<typename T>
class Val;

template<typename>
struct isVal : std::false_type {};

template<typename T>
struct isVal<Val<T>> : std::true_type {
    using type = T;
};

template<class T>
class Val {
public:
    template<typename U>
    static constexpr bool is_void = std::is_same_v<std::remove_const_t<U>, void>;

    template<typename U>
    static constexpr bool is_const = std::is_const_v<U>;

    template<typename U>
    using non_const = std::remove_const_t<U>;

    template<typename V>
    Val(const Val<V>& other) : _data(other._data){}
    template<typename V, typename U = T, std::enable_if_t<(!is_void<U>), bool> = true>
    Val(const V& data){_data = std::make_shared<T>(data);}

    template<typename V>
    Val(std::shared_ptr<V> data) : _data(data){
        if constexpr (!is_void<T>){
            if (!data){
                throw std::logic_error("non-void glwpp::util::Val can not be nullptr");
            }
        }
    }


    virtual ~Val(){};
    

    template<typename V, typename U = T, std::enable_if_t<(!is_void<U> && !(is_const<U> && !is_const<V>)), bool> = true>
    operator V&&() const {return *_data;}
    template<typename V, typename U = T, std::enable_if_t<(!is_void<U>), bool> = true>
    operator V() const {return *_data;}
    template<typename V>
    operator V*() const {return _data.get();}


    T* operator->() const {return _data.get();}
    template<typename U = T, std::enable_if_t<(!is_void<U>), bool> = true>
    U& operator*() const {return *_data;}


    template<typename D>
    Val<D> cast_static() const {return Val<D>(std::static_pointer_cast<D>(_data));}

    template<typename D>
    Val<D> cast_dynamic() const {return Val<D>(std::dynamic_pointer_cast<D>(_data));}

    template<typename D>
    Val<D> cast_reinterpret() const {return Val<D>(std::reinterpret_pointer_cast<D>(_data));}

    std::shared_ptr<T> get() const {return _data;}

private:
    std::shared_ptr<T> _data;
    
    template<typename>
    friend class Val;
};

} // namespace glwpp
