#pragma once

#include <memory>

namespace nglpmt::native {

template<typename T>
class SharedObject : public std::enable_shared_from_this<T> {
public:
    static std::shared_ptr<T> make(auto&&... args){
        return T::make(std::forward<decltype(args)>(args)...);
    };
    virtual ~SharedObject(){};

    

protected:
    SharedObject(){};
};

}