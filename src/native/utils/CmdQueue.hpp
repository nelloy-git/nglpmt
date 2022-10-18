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

    size_t size() const;
    void push_back(const std::function<void()>& cmd);
    void push_front(const std::function<void()>& cmd);

protected:
    CmdQueue(const std::shared_ptr<BS::thread_pool> pool);

private:
    std::shared_ptr<std::mutex> _lock;
    std::shared_ptr<std::atomic<bool>> _is_executing;
    std::shared_ptr<std::deque<std::function<void()>>> _queue;
    std::shared_ptr<BS::thread_pool> _pool;

    void _update();
};

} //namespace nglpmt::native 