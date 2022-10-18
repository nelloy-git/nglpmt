#pragma once

#include "native/ContextObject.hpp"
#include "native/gl/Types.hpp"
#include "native/utils/Val.hpp"

namespace nglpmt::native {

class GlObjectStatic {
public:
    static std::optional<std::string> getGlMessage(const SrcLoc& src_loc);
    inline static void debug(const SrcLoc& src_loc){
#ifdef NGLPMT_DEBUG
    auto msg = getGlMessage(src_loc);
    if (msg.has_value()){
        std::cout << msg.value().c_str() << std::endl;
    }
#endif
    }
};

template<typename T>
class GlObject : public ContextObject<T>, public GlObjectStatic {
public:
    virtual ~GlObject(){};

    const Val<const UInt> id() const {
        return _id;
    }

protected:
    GlObject(const std::shared_ptr<Context>& ctx,
             auto&& initer, auto&& deleter, auto&&... args) :
        ContextObject<T>(ctx),
        _id(_make_id(ctx, deleter)){
        if (!this->isContextThread()){
            auto tuple_args = std::make_tuple(_id, args...);
            ctx->onRun->addActionQueued([initer, tuple_args](){
                std::apply(initer, tuple_args);
                return false;
            });
        } else {
            initer(_id, std::forward<decltype(args)>(args)...);
        }
    };
    
private:
    Val<UInt> _id;

    static std::shared_ptr<UInt> _make_id(const std::shared_ptr<Context>& ctx,
                                          auto&& deleter){
        return std::shared_ptr<UInt>(new UInt(0), [wctx = std::weak_ptr<Context>(ctx), deleter](UInt* id){
            auto ctx = wctx.lock();
            if (!ctx || *id == 0){
                delete id;
                return;
            }

            if (ctx->getThreadId() == std::this_thread::get_id()){
                deleter(*id);
            } else {
                ctx->onRun->addActionQueued([deleter, id](){
                    deleter(*id);
                    return false;
                });
            }
            delete id;
            return;
        });
    };
};


} // namespace nglpmt::native