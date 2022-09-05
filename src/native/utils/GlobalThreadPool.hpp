#pragma once

#include "BS_thread_pool.hpp"

namespace nglpmt::native {

class GlobalThreadPool {
public:
    static std::shared_ptr<BS::thread_pool> get(){
        std::lock_guard lg(_getLock());

        auto& wptr = _getWeak();
        auto sptr = wptr.lock();
        if (sptr){return sptr;}

        sptr = std::make_shared<BS::thread_pool>();
        wptr = sptr;
        return sptr;
    }

private:
    GlobalThreadPool(){};
    ~GlobalThreadPool(){};
    
    static std::mutex& _getLock(){
        static std::mutex lock;
        return lock;
    }

    static std::weak_ptr<BS::thread_pool>& _getWeak(){
        static std::weak_ptr<BS::thread_pool> ptr;
        return ptr;
    }
};

} // namespace nglpmt