#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace nglpmt {

template<typename K, typename V>
class MapMT {
public:
    MapMT(){};
    virtual ~MapMT(){};

    void set(const K& key, std::shared_ptr<V> value){
        std::lock_guard lg(_lock);
        _map[key] = value;
    }

    std::shared_ptr<V> get(const K& key) const {
        std::lock_guard lg(_lock);
        auto iter = _map.find(key);
        return iter != _map.end()
                ? iter->second.lock()
                : std::shared_ptr<V>();
    }

    std::shared_ptr<V> getOrMake(const K& key,
                                 const auto& constructor,
                                 auto&&... args){
        std::lock_guard lg(_lock);
        auto iter = _map.find(key);

        std::shared_ptr<V> result;
        if (iter != _map.end()){
            result = iter->second.lock();
        }

        if (!result){
            result = constructor(std::forward<decltype(args)>(args)...);
            _map[key] = result;
        }

        return result;
    }

private:
    std::mutex _lock;
    std::unordered_map<K, std::weak_ptr<V>> _map;

};

} // namespace nglpmt