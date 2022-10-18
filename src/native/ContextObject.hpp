#pragma once

#include "native/Context.hpp"
#include "native/utils/SharedObject.hpp"

namespace nglpmt::native {

template<typename T>
class ContextObject : public SharedObject<T> {
public:
    virtual ~ContextObject(){};

    inline const std::weak_ptr<Context>& getContext() const {
        return _wctx;
    }

    inline bool isContextThread() const {
        return std::this_thread::get_id() == _ctx_thread_id;
    };

    template<auto M>
    inline bool movedToContext(auto&&... args){
        if (isContextThread()){
            return false;
        }

        auto ctx = _wctx.lock();
        if (ctx){
            ctx->onRun->addActionQueued([self = this->shared_from_this(), args...](){
                std::invoke(M, self.get(), args...);
                return false;
            });
        } else {
            throw std::runtime_error("Context is destroyed");
        }
        return true;
    }

    template<auto M>
    inline bool movedToContext(auto&&... args) const {
        if (isContextThread()){
            return false;
        }

        auto ctx = _wctx.lock();
        if (ctx){
            auto tuple_args = std::make_tuple(std::forward<decltype(args)>(args)...);
            ctx->onRun->addActionQueued([self = this->shared_from_this(), tuple_args](){
                apply_invoke(M, self.get(), tuple_args);
                return false;
            });
        } else {
            throw std::runtime_error("Context is destroyed");
        }
        return true;
    }

protected:
    ContextObject(const std::shared_ptr<Context>& ctx) :
        SharedObject<T>(),
        _wctx(ctx),
        _ctx_thread_id(ctx->getThreadId()){
    }

private:
    const std::weak_ptr<Context> _wctx;
    std::thread::id _ctx_thread_id;

    template<typename F, typename C, typename U>
    static decltype(auto) apply_invoke(F&& func, C&& first, U&& tuple){
        return std::apply(std::forward<F>(func), std::tuple_cat(std::forward_as_tuple(std::forward<C>(first)), std::forward<U>(tuple)));
    }
};

} // namespace nglpmt::native