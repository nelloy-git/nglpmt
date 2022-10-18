#include "native/utils/CmdQueue.hpp"

using namespace nglpmt::native;

std::shared_ptr<CmdQueue> CmdQueue::make(const std::shared_ptr<BS::thread_pool> pool){
    return std::shared_ptr<CmdQueue>(new CmdQueue(pool));
}

CmdQueue::CmdQueue(const std::shared_ptr<BS::thread_pool> pool) :
    _lock(new std::mutex),
    _is_executing(new std::atomic<bool>(false)),
    _queue(new std::deque<std::function<void()>>),
    _pool(pool){
}

CmdQueue::~CmdQueue(){
    _is_executing->wait(true);
}

size_t CmdQueue::size() const {
    std::lock_guard lg(*_lock);
    return _queue->size();
}

void CmdQueue::push_back(const std::function<void()>& cmd){
    std::lock_guard lg(*_lock);
    _queue->push_back(cmd);
    _update();
}

void CmdQueue::push_front(const std::function<void()>& cmd){
    std::lock_guard lg(*_lock);
    _queue->push_front(cmd);
    _update();
}

// Should be called when mutex is locked
void CmdQueue::_update(){
    bool executing = false;
    while(!_is_executing->compare_exchange_weak(executing, true) && !executing);

    if (!executing && _queue->size() > 0){
        static_cast<void>(_pool->submit([lock = _lock, is_executing = _is_executing, queue = _queue](){
            std::function<void()> cmd;
            while(true){
                {
                    std::lock_guard lg(*lock);
                    if (queue->size() <= 0){
                        *is_executing = false;
                        is_executing->notify_one();
                        break;
                    }
                    cmd = queue->front();
                    queue->pop_front();
                }
                cmd();
            }
        }));
    }
}