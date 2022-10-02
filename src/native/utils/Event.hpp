#pragma once

#include <atomic>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "native/utils/CmdQueue.hpp"
#include "native/utils/FuncWrapper.hpp"
#include "native/utils/GlobalThreadPool.hpp"
#include "native/utils/SharedObject.hpp"
#include "native/utils/SrcLoc.hpp"

namespace nglpmt::native {

template<typename ... Args>
class Event : public SharedObject<Event<Args...>> {
    struct ActionData {
        ActionData(const auto& action, const SrcLoc& src_loc) :
            func(std::function(expand_func<Args...>(action))),
            src_loc(src_loc){
        }
        std::function<bool(Args...)> func;
        SrcLoc src_loc;
    };

public:
    using ID = uint64_t;

    static std::shared_ptr<Event> make(const std::shared_ptr<BS::thread_pool> emit_pool = GlobalThreadPool::get(),
                                       const std::shared_ptr<BS::thread_pool> manager_pool = GlobalThreadPool::get()){
        return std::shared_ptr<Event>(new Event<Args...>(emit_pool, manager_pool));
    }
    Event(const Event&) = delete;
    Event(const Event&&) = delete;
    virtual ~Event();

    ID addActionQueued(const auto& action, const SrcLoc& src_loc = SrcLoc{});
    ID addActionNow(const auto& action, const SrcLoc& src_loc = SrcLoc{});

    std::future<bool> delActionQueued(const ID& id);
    std::future<bool> delActionNow(const ID& id);

    std::future<void> emitQueued(const Args&... args, const SrcLoc& src_loc = SrcLoc{});
    std::future<void> emitNow(const Args&... args, const SrcLoc& src_loc = SrcLoc{});

protected:
    Event(const std::shared_ptr<BS::thread_pool> emit_pool,
          const std::shared_ptr<BS::thread_pool> manager_pool);

private:
    std::mutex _lock;
    std::atomic<ID> _action_id;
    std::unordered_map<ID, std::shared_ptr<ActionData>> _actions_in_use;
    std::shared_ptr<CmdQueue> _manager;
    std::shared_ptr<BS::thread_pool> _emit_pool;

    void _pushExecuteEmit(std::shared_ptr<std::promise<void>> promise, const Args&... args);
    void _executeEmit(std::shared_ptr<std::promise<void>> promise, const Args&... args);
    void _executeDelete(const ID& id, std::shared_ptr<std::promise<bool>> promise);
};





template<typename ... Args>
inline Event<Args...>::Event(const std::shared_ptr<BS::thread_pool> emit_pool,
                               const std::shared_ptr<BS::thread_pool> manager_pool) :
    _action_id(0),
    _manager(CmdQueue::make(manager_pool)),
    _emit_pool(emit_pool){
    _emit_pool->unpause();
}

template<typename ... Args>
inline Event<Args...>::~Event<Args...>(){
}

template<typename ... Args>
inline std::future<void> Event<Args...>::emitQueued(const Args&... args,
                                             const SrcLoc& src_loc){
    auto weak_self = this->weak_from_this();
    auto promise = std::make_shared<std::promise<void>>();

    _manager->push_back([weak_self, promise, args...](){
        auto self = weak_self.lock();
        if (!self){return;}
        self->_pushExecuteEmit(promise, args...);
    });

    return promise->get_future();
}

template<typename ... Args>
inline std::future<void> Event<Args...>::emitNow(const Args&... args,
                                          const SrcLoc& src_loc){
    auto weak_self = this->weak_from_this();
    auto promise = std::make_shared<std::promise<void>>();

    _manager->push_front([weak_self, promise, args...](){
        std::cout << __FUNCTION__ << std::endl;
        auto self = weak_self.lock();
        if (!self){return;}
        self->_pushExecuteEmit(promise, args...);
    });

    return promise->get_future();
}

template<typename ... Args>
inline std::future<bool> Event<Args...>::delActionQueued(const ID& id){
    auto weak_self = this->weak_from_this();
    auto promise = std::make_shared<std::promise<bool>>();

    _manager->push_back([weak_self, id, promise](){
        auto self = weak_self.lock();
        if (!self){return;}
        self->_executeDelete(id, promise);
    });
    return promise->get_future();
}

template<typename ... Args>
inline std::future<bool> Event<Args...>::delActionNow(const ID& id){
    auto weak_self = this->weak_from_this();
    auto promise = std::make_shared<std::promise<bool>>();

    _manager->push_front([weak_self, id, promise](){
        auto self = weak_self.lock();
        if (!self){return;}
        (self->_executeDelete(id, promise));
    });
    return promise->get_future();
}

template<typename ... Args>
inline Event<Args...>::ID Event<Args...>::addActionQueued(const auto& action,
                                        const SrcLoc& src_loc){
    auto weak_self = this->weak_from_this();
    auto id = _action_id.fetch_add(1);
    auto data = std::make_shared<ActionData>(action, src_loc);

    _manager->push_back([weak_self, id, data](){
        std::cout << __FUNCTION__ << std::endl;
        auto self = weak_self.lock();
        if (!self){return;}

        std::lock_guard lg(self->_lock);
        self->_actions_in_use[id] = data;
    });

    return id;
}

template<typename ... Args>
inline Event<Args...>::ID Event<Args...>::addActionNow(const auto& action,
                                           const SrcLoc& src_loc){
    auto weak_self = this->weak_from_this();
    auto id = _action_id.fetch_add(1);
    auto data = std::make_shared<ActionData>(action, src_loc);

    _manager->push_front([weak_self, id, data](){
        std::cout << __FUNCTION__ << std::endl;
        auto self = weak_self.lock();
        if (!self){return;}

        std::lock_guard lg(self->_lock);
        self->_actions_in_use[id] = data;
    });

    return id;
}

template<typename ... Args>
inline void Event<Args...>::_pushExecuteEmit(std::shared_ptr<std::promise<void>> promise, const Args&... args){
    auto weak_self = this->weak_from_this();
    static_cast<void>(_emit_pool->submit([weak_self, promise, args...](){
        std::cout << __FUNCTION__ << std::endl;
        std::shared_ptr<Event<Args...>> self = weak_self.lock();
        if (!self){
            promise->set_value();
            return;
        }
        self->_executeEmit(promise, args...);
    }));
}

template<typename ... Args>
inline void Event<Args...>::_executeEmit(std::shared_ptr<std::promise<void>> promise, const Args&... args){
    std::lock_guard lg(_lock);
    std::unordered_map<ID, std::shared_ptr<ActionData>> new_map;
    for (auto& data : _actions_in_use){
        std::cout << __FUNCTION__ << std::endl;
        bool repeat = false;
        try {
            repeat = data.second->func(args...);
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }

        if (repeat){
            new_map[data.first] = data.second;
        }
    }
    _actions_in_use.clear();
    _actions_in_use.swap(new_map);
    promise->set_value();
}

template<typename ... Args>
inline void Event<Args...>::_executeDelete(const ID& id, std::shared_ptr<std::promise<bool>> promise){
    std::lock_guard lg(_lock);
    auto iter = _actions_in_use.find(id);
    bool found = iter != _actions_in_use.end();
    if (found){
        _actions_in_use.erase(iter);
    }
    std::cout << __FUNCTION__ << " set_value" << std::endl;
    try {
        promise->set_value(found);
    } catch (const std::exception& e){
        std::cout << e.what() << std::endl;
    }
    std::cout << __FUNCTION__ << " set_value end" << std::endl;
}

} // namespace nglpmt::native