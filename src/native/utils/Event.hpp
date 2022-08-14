#pragma once

#include <atomic>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>

#include "BS_thread_pool.hpp"
#include "native/utils/FuncWrapper.hpp"
#include "native/utils/SrcLoc.hpp"

namespace nglpmt::native {

template<typename ... Args>
class Event final {
    using Condition = std::function<bool(Args...)>;
    using Action = std::function<void(Args...)>;
    using ActionData = std::tuple<std::shared_ptr<Condition>, std::shared_ptr<Action>, std::shared_ptr<SrcLoc>>;

public:
    Event(const std::shared_ptr<BS::thread_pool> th_pool = _getDefaultPool()){
        _pool = th_pool;
        _list = std::make_shared<std::deque<ActionData>>();
    }
    Event(const Event&) = delete;
    Event(const Event&&) = delete;
    ~Event(){}

    // Both action and condition are constexpr
    template<auto F, auto C = nullptr>
    void push(const SrcLoc& loc = SrcLoc{}){
        static constexpr auto expanded_condition = _expandCondition<C>();
        static constexpr auto expanded_action = expand_func<F, Args...>();
        
        std::shared_ptr<Condition> condition;
        if constexpr (C != nullptr){
            condition = std::make_shared<Condition>(expanded_condition);
        }
        auto action = std::make_shared<Action>(expanded_action);
        auto src_loc = std::make_shared<SrcLoc>(loc);

        _push(condition, action, src_loc);
    }

    // Condition is constexpr, emit once by default
    template<auto C = nullptr, class F>
    void push(const F& event_action, const SrcLoc& loc = SrcLoc{}){
        static constexpr auto expanded_condition = _expandCondition<C>();
        auto expanded_action = expand_func<Args...>(event_action);
        
        std::shared_ptr<Condition> condition;
        if constexpr (C != nullptr){
            condition = std::make_shared<Condition>(expanded_condition);
        }
        auto action = std::make_shared<Action>(expanded_action);
        auto src_loc = std::make_shared<SrcLoc>(loc);

        _push(condition, action, src_loc);
    }

    // Everything is runtime
    template<class F, class C>
    void push(const F& event_action, const C& alive_condition, const SrcLoc& loc = SrcLoc{}){
        auto expanded_condition = expand_func<Args...>(alive_condition);
        auto expanded_action = expand_func<Args...>(event_action);

        auto condition = std::make_shared<Condition>(expanded_condition);
        auto action = std::make_shared<Action>(expanded_action);
        auto src_loc = std::make_shared<SrcLoc>(loc);

        _push(condition, action, src_loc);
    }

    std::future<void> emit(const Args&... args){
        auto divided = _divideList(args...);
        return _pool->submit([divided](const Args&... args){
            for (auto& data : *divided.to_do){
                try {
                    (*std::get<1>(data))(args...);
                } catch (const std::exception& e){
                    // TODO messaging
                    std::cout << e.what() << std::endl;
                }
            }
        }, args...);
    }

    size_t size(){
        std::lock_guard lg(_lock);
        return _list->size();
    }

private:
    std::shared_ptr<BS::thread_pool> _pool;

    std::mutex _lock;
    std::shared_ptr<std::deque<ActionData>> _list;

    void _push(const std::shared_ptr<std::function<bool(Args...)>>& condition,
               const std::shared_ptr<std::function<void(Args...)>>& action,
               const std::shared_ptr<SrcLoc>& loc){
        std::lock_guard lg(_lock);
        _list->push_back(std::make_tuple(condition, action, loc));
    }

    struct DividedList {
        std::shared_ptr<std::deque<ActionData>> to_do;
        std::shared_ptr<std::deque<ActionData>> to_clear;
    };

    DividedList _divideList(const Args&... args){
        DividedList divided;
        divided.to_do = std::make_shared<std::deque<ActionData>>();
        divided.to_clear = std::make_shared<std::deque<ActionData>>();

        std::lock_guard lg(_lock);
        auto iter = _list->begin();
        while(iter != _list->end()){
            auto &data = *iter;
            auto cond = std::get<0>(data);
            // No condition => execute once
            if (!cond){
                divided.to_do->push_back(*iter);
                iter = _list->erase(iter);
                continue;
            } else {
                bool alive = (*cond)(args...);
                if (alive){
                    divided.to_do->push_back(*iter);
                    ++iter;
                } else {
                    divided.to_clear->push_back(*iter);
                    iter = _list->erase(iter);
                }
            }
        }

        return divided;
    }

    static std::mutex _default_pool_lock;
    static std::weak_ptr<BS::thread_pool> _default_pool;

    static std::shared_ptr<BS::thread_pool> _getDefaultPool(){
        std::lock_guard lg(_default_pool_lock);

        auto ptr = _default_pool.lock();
        if (ptr){return ptr;}

        ptr = std::make_shared<BS::thread_pool>();
        _default_pool = ptr;
        return ptr;
    }

    template<auto C>
    static constexpr auto _expandCondition(){
        if constexpr (C != nullptr){
            return expand_func<C, Args...>();
        } else {
            return nullptr;
        }
    }
};

template<typename ... Args>
std::mutex Event<Args...>::_default_pool_lock;

template<typename ... Args>
std::weak_ptr<BS::thread_pool> Event<Args...>::_default_pool;

}