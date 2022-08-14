#pragma once

#include "native/Context.hpp"
#include "native/utils/Val.hpp"
#include "native/utils/SrcLoc.hpp"

namespace nglpmt::native {

template<typename T>
class ContextObject : public std::enable_shared_from_this<T> {
public:
    static std::shared_ptr<T> make(const sptr<Context>& ctx,
                                   auto&&... args){
        return T::make(ctx, std::forward<decltype(args)>(args)...);
    };
    virtual ~ContextObject(){};

    const std::weak_ptr<Context>& getContext() const {
        return _weak_ctx;
    }

    bool isContextThread() const {
        return std::this_thread::get_id() == _ctx_thread_id;
    };

    bool executeInContext(auto&& func, auto&&... args) const {
        auto ctx = _weak_ctx.lock();
        if (!ctx) return false;
        ctx->onRun.push([=](){
            func(args...);
        });
        return true;
    }

    template<typename M>
    bool executeMethodInContext(auto&&... args){
        return executeInContext(&ContextObject::_executeMethod<M>,
                                shared_from_this(), std::forward<decltype(args)>(args)...);
    }

    static std::atomic<std::function<void(const std::string&, const utils::SrcLoc&)>*> debug_function;

protected:
    ContextObject(const sptr<Context>& ctx);

private:
    const std::shared_ptr<Context> _weak_ctx;
    std::thread::id _ctx_thread_id;

    template<typename M>
    static inline void _executeMethod(const std::shared_ptr<T>& this_ptr, auto&&... args){
        (this_ptr.get()->*M)(std::forward<decltype(args)>(args)...);
    }
};

}