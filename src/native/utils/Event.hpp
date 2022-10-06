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
    using IdMap = std::unordered_map<ID, std::shared_ptr<ActionData>>;

    static std::shared_ptr<Event> make(const std::shared_ptr<BS::thread_pool> emitter_pool = GlobalThreadPool::get()){
        return std::shared_ptr<Event>(new Event<Args...>(emitter_pool));
    }
    Event(const Event&) = delete;
    Event(const Event&&) = delete;
    virtual ~Event();

    std::pair<ID, std::future<void>> addActionQueued(const auto& action, const SrcLoc& src_loc = SrcLoc{});
    std::pair<ID, std::future<void>> addActionNow(const auto& action, const SrcLoc& src_loc = SrcLoc{});

    std::future<bool> delActionQueued(const ID& id);
    std::future<bool> delActionNow(const ID& id);

    std::future<void> emitQueued(const Args&... args, const SrcLoc& src_loc = SrcLoc{});
    std::future<void> emitNow(const Args&... args, const SrcLoc& src_loc = SrcLoc{});

protected:
    Event(const std::shared_ptr<BS::thread_pool> emitter_pool);

private:
    std::shared_ptr<std::mutex> _lock;
    std::shared_ptr<CmdQueue> _emitter;
    std::shared_ptr<IdMap> _active;
    std::shared_ptr<IdMap> _queued;

    static ID _getUniqId();

    static bool _addAction(const ID& id, std::mutex& lock, IdMap& queued, IdMap& active){
        std::lock_guard lg(lock);
        auto iter = queued.find(id);
        if (iter == queued.end()){
            return false;
        }
        active.insert_or_assign(iter->first, iter->second);
        queued.erase(iter);
        return true;
    }

    static bool _removeAction(const ID& id, std::mutex& lock, IdMap& queued, IdMap& active){
        std::lock_guard lg(lock);
        auto queued_iter = queued.find(id);
        bool queued_found = (queued_iter != queued.end());
        if (queued_found){
            queued.erase(queued_iter);
        }

        auto active_iter = active.find(id);
        bool active_found = (active_iter != active.end());
        if (active_found){
            active.erase(active_iter);
        }

        return queued_found || active_found;
    }

    static void _emitActions(std::mutex& lock, IdMap& active, Args... args){
        std::lock_guard lg(lock);
        IdMap new_map;
        for (auto& data : active){
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
        active = new_map;
    }
};





template<typename ... Args>
inline Event<Args...>::Event(const std::shared_ptr<BS::thread_pool> emitter_pool) :
    _lock(new std::mutex),
    _emitter(CmdQueue::make(emitter_pool)),
    _active(new IdMap),
    _queued(new IdMap){
}

template<typename ... Args>
inline Event<Args...>::~Event(){
}

template<typename ... Args>
inline std::pair<typename Event<Args...>::ID, std::future<void>>
Event<Args...>::addActionQueued(const auto& action,
                                const SrcLoc& src_loc){
    auto id = _getUniqId();
    auto data = std::make_shared<ActionData>(action, src_loc);
    auto promise = std::make_shared<std::promise<void>>();

    {
        std::lock_guard lg(*_lock);
        _queued->insert_or_assign(id, data);
    }
    
    _emitter->push_back([id, promise, lock = _lock, active = _active, queued = _queued](){
        _addAction(id, *lock, *queued, *active);
        promise->set_value();
    });

    return std::make_pair(id, promise->get_future());
}

template<typename ... Args>
inline std::pair<typename Event<Args...>::ID, std::future<void>>
Event<Args...>::addActionNow(const auto& action,
                             const SrcLoc& src_loc){
    auto id = _getUniqId();
    auto data = std::make_shared<ActionData>(action, src_loc);
    auto promise = std::make_shared<std::promise<void>>();

    {
        std::lock_guard lg(*_lock);
        _queued->insert_or_assign(id, data);
    }

    _emitter->push_front([id, promise, lock = _lock, active = _active, queued = _queued](){
        _addAction(id, *lock, *queued, *active);
        promise->set_value();
    });

    return std::make_pair(id, promise->get_future());
}

template<typename ... Args>
inline std::future<bool>
Event<Args...>::delActionQueued(const ID& id){
    auto promise = std::make_shared<std::promise<bool>>();
    _emitter->push_back([id, promise, lock = _lock, active = _active, queued = _queued](){
        bool found = _removeAction(id, *lock, *queued, *active);
        promise->set_value(found);
    });
    return promise->get_future();
}

template<typename ... Args>
inline std::future<bool>
Event<Args...>::delActionNow(const ID& id){
    std::cout << __FUNCTION__ << std::endl;
    auto promise = std::make_shared<std::promise<bool>>();
    _emitter->push_front([id, promise, lock = _lock, active = _active, queued = _queued](){
        bool found = _removeAction(id, *lock, *queued, *active);
        std::cout << __FUNCTION__ << (found ? " true" : " false") << std::endl;
        promise->set_value(found);
    });
    return promise->get_future();
}

template<typename ... Args>
inline std::future<void>
Event<Args...>::emitQueued(const Args&... args,
                           const SrcLoc& src_loc){
    auto promise = std::make_shared<std::promise<void>>();
    _emitter->push_back([promise, lock = _lock, active = _active, args...](){
        _emitActions(*lock, *active, args...);
        promise->set_value();
    });

    return promise->get_future();
}

template<typename ... Args>
inline std::future<void>
Event<Args...>::emitNow(const Args&... args,
                        const SrcLoc& src_loc){
    auto promise = std::make_shared<std::promise<void>>();
    _emitter->push_front([promise, lock = _lock, active = _active, args...](){
        _emitActions(*lock, *active, args...);
        promise->set_value();
    });

    return promise->get_future();
}

template<typename ... Args>
inline Event<Args...>::ID Event<Args...>::_getUniqId(){
    static std::atomic<ID> id(0);
    return id.fetch_add(1);
}

} // namespace nglpmt::native