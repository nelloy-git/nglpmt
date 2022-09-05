#pragma once

#include <atomic>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>

#include "BS_thread_pool.hpp"
#include "native/utils/FuncWrapper.hpp"
#include "native/utils/GlobalThreadPool.hpp"
#include "native/utils/SharedObject.hpp"
#include "native/utils/SrcLoc.hpp"

namespace nglpmt::native {

enum class EventConditionResult {
    Continue,   // run action and add to next emit queue
    Once,       // run action once and forgive
    Next,       // do not run action but add to next emit queue
    Free,       // do not run action and forgive
    Retry,      // do not run action and move to the end of the current emit queue
};

template<typename ... Args>
class EventData {
public:
    using Condition = std::function<EventConditionResult(Args...)>;
    using Action = std::function<void(Args...)>;

    EventData(const Action& action,
              const std::optional<Condition>& condition = std::nullopt,
              const SrcLoc& src_loc = SrcLoc{}) :
        action(action),
        condition(condition),
        src_loc(src_loc){
    }
    virtual ~EventData(){};

    inline EventConditionResult checkCondition(const Args&... args){
        if (!condition.has_value()){
            return EventConditionResult::Once;
        }

        try {
            return condition.value()(args...);
        } catch (const std::exception& e){
            std::cout << e.what() << std::endl << src_loc.to_string() << std::endl;
            return EventConditionResult::Free;
        }
    }

    inline void runAction(const Args&... args){
        try {
            action(args...);
        } catch (const std::exception& e){
            std::cout << e.what() << std::endl << src_loc.to_string() << std::endl;
        }
    }

    Action action;
    std::optional<Condition> condition;
    SrcLoc src_loc;
};

template<typename ... Args>
class Event : public SharedObject<Event<Args...>> {
public:
    static std::shared_ptr<Event> make(const std::shared_ptr<BS::thread_pool> action_thread_pool = GlobalThreadPool::get(),
                                       const std::shared_ptr<BS::thread_pool> condition_thread_pool = GlobalThreadPool::get()){
        return std::shared_ptr<Event>(new Event(action_thread_pool, condition_thread_pool));
    }
    Event(const Event&) = delete;
    Event(const Event&&) = delete;
    virtual ~Event(){}

    // Both action and condition are constexpr
    template<auto F, auto C = nullptr>
    void push(const SrcLoc& src_loc = SrcLoc{}){
        static constexpr auto expanded_action = expand_func<F, Args...>();
        
        std::lock_guard lg(_lock);
        if constexpr (C == nullptr){
            _data_list->emplace_back(expanded_action, std::nullopt, src_loc);
        } else {
            static constexpr auto expanded_condition = expand_func<C, Args...>();
            _data_list->emplace_back(expanded_action, expanded_condition, src_loc);
        }
    }

    // Condition is constexpr, emit once by default
    template<auto C = nullptr, class F>
    void push(const F& event_action, const SrcLoc& src_loc = SrcLoc{}){
        auto expanded_action = expand_func<Args...>(event_action);
        
        std::lock_guard lg(_lock);
        if constexpr (C == nullptr){
            _data_list->emplace_back(expanded_action, std::nullopt, src_loc);
        } else {
            static constexpr auto expanded_condition = expand_func<C, Args...>();
            _data_list->emplace_back(expanded_action, expanded_condition, src_loc);
        }
    }

    // Everything is runtime
    template<class F, class C>
    void push(const F& event_action, const C& alive_condition, const SrcLoc& src_loc = SrcLoc{}){
        auto expanded_action = expand_func<Args...>(event_action);
        auto expanded_condition = expand_func<Args...>(alive_condition);

        std::lock_guard lg(_lock);
        _data_list->emplace_back(expanded_action, expanded_condition, src_loc);
    }

    void emit(const Args&... args){
        std::lock_guard lg(_lock);

        auto self = this->shared_from_this();
        auto promise = _condition_thread_pool->submit([self](const Args&... args){
            std::lock_guard lg(self->_lock);

            auto new_list = std::make_shared<std::deque<EventData<Args...>>>();
            auto action_list = std::make_shared<std::deque<EventData<Args...>>>();

            while (!self->_data_list->empty()){
                auto data = self->_data_list->front();
                self->_data_list->pop_front();
                auto res = data.checkCondition(args...);
                switch (res){
                case EventConditionResult::Free:
                    break;
                    
                case EventConditionResult::Next:
                    new_list->push_back(data);
                    break;

                case EventConditionResult::Once:
                    action_list->push_back(data);
                    break;

                case EventConditionResult::Continue:
                    new_list->push_back(data);
                    action_list->push_back(data);
                    break;

                case EventConditionResult::Retry:
                    self->_data_list->push_back(data);
                default:
                    break;
                }
            }
            self->_data_list = new_list;

            auto promise = self->_action_thread_pool->submit([action_list](const Args&... args){
                for (auto& data : *action_list){
                    data.runAction(args...);
                }
            }, args...);
        }, args...);
    }

    size_t size(){
        std::lock_guard lg(_lock);
        return _data_list->size();
    }

protected:
    Event(const std::shared_ptr<BS::thread_pool> action_thread_pool,
          const std::shared_ptr<BS::thread_pool> condition_thread_pool) :
        _data_list(new std::deque<EventData<Args...>>),
        _action_thread_pool(action_thread_pool),
        _condition_thread_pool(condition_thread_pool){
    }

private:
    std::mutex _lock;
    std::shared_ptr<std::deque<EventData<Args...>>> _data_list;
    std::shared_ptr<BS::thread_pool> _action_thread_pool;
    std::shared_ptr<BS::thread_pool> _condition_thread_pool;
};

}