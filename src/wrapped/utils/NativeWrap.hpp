#pragma once

#include "napi.h"

namespace nglpmt::js {

template<typename T>
class NativeWrap : public Napi::ObjectWrap<NativeWrap<T>> {
public:
    static const std::pair<std::string, Napi::Function>& getJsConstructor(Napi::Env env){
        static std::unordered_map<Napi::Env, std::pair<std::string, Napi::Function>> constructors;
        auto iter = constructors.find(env);
        if (iter == constructors.end()){
            constructors.emplace(std::move(createJsConstructor(env)));
            return constructors[env];
        }
        return iter->second;
    }
    
    static const std::pair<std::string, Napi::Function> createJsConstructor(Napi::Env env){
        return T::createJsConstructor(env);
    }

    NativeWrap(const Napi::CallbackInfo& info, std::shared_ptr<T> native) :
        Napi::ObjectWrap<NativeWrap<T>>(info),
        _native(native){
    };
    ~NativeWrap(){};

    std::shared_ptr<T> getNative(){
        return _native;
    }

private:
    std::shared_ptr<T> _native;
};

} // namespace nglpmt::js