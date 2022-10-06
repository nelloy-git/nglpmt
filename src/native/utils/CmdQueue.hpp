#pragma once

#include <atomic>
#include <deque>
#include <memory>

#include "BS_thread_pool.hpp"

#include "native/utils/SharedObject.hpp"

namespace nglpmt::native {

class CmdQueue : public SharedObject<CmdQueue> {
public:
    static std::shared_ptr<CmdQueue> make(const std::shared_ptr<BS::thread_pool> pool);
    CmdQueue(const CmdQueue&) = delete;
    CmdQueue(const CmdQueue&&) = delete;
    ~CmdQueue();

    void push_back(const std::function<void()>& cmd);
    void push_front(const std::function<void()>& cmd);

protected:
    CmdQueue(const std::shared_ptr<BS::thread_pool> pool);

private:
    std::mutex _lock;
    std::atomic<bool> _is_executing;
    std::deque<std::function<void()>> _queue;
    std::shared_ptr<BS::thread_pool> _pool;

    void _update();
    void _pushPool();
    void _execute();
};

} //namespace nglpmt::native 