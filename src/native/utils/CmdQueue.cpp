#include "native/utils/CmdQueue.hpp"

using namespace nglpmt::native;

std::shared_ptr<CmdQueue> CmdQueue::make(const std::shared_ptr<BS::thread_pool> pool){
    return std::shared_ptr<CmdQueue>(new CmdQueue(pool));
}

CmdQueue::CmdQueue(const std::shared_ptr<BS::thread_pool> pool) :
    _is_executing(false),
    _pool(pool){
}

CmdQueue::~CmdQueue(){
    _is_executing.wait(true);
}

void CmdQueue::push_back(const std::function<void()>& cmd){
    std::lock_guard lg(_lock);
    _queue.push_back(cmd);
    _update();
}

void CmdQueue::push_front(const std::function<void()>& cmd){
    std::lock_guard lg(_lock);
    _queue.push_front(cmd);
    _update();
}

// Should be called when mutex is locked
void CmdQueue::_update(){
    bool executing = false;
    while(!_is_executing.compare_exchange_weak(executing, true) && !executing);

    if (!executing && _queue.size() > 0){
        std::cout << "Push" << std::endl;
        _pushPool();
    }
}

void CmdQueue::_pushPool(){
    static_cast<void>(_pool->submit([self = this->shared_from_this()](){
        self->_execute();
    }));
}

void CmdQueue::_execute(){
    std::function<void()> cmd;
    {
        std::lock_guard lg(_lock);
        cmd = _queue.front();
    }

    std::cout << "start" << std::endl;
    cmd();
    std::cout << "finish" << std::endl;

    std::lock_guard lg(_lock);
    _queue.pop_front();
    if (_queue.size() > 0){
        _pushPool();
    } else {
        std::cout << "end" << std::endl;
        _is_executing = false;
        _is_executing.notify_one();
    }
}