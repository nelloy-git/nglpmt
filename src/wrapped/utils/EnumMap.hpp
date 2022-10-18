#pragma once

#include <optional>
#include <unordered_map>

namespace nglpmt::js {

template<typename T>
class EnumMap {
    using Map = std::unordered_map<std::string, T>;

public:
    EnumMap(const std::initializer_list<typename Map::value_type>& _Ilist) :
        _map(_Ilist),
        _available(_getAvailable()){
    }
    EnumMap(const EnumMap&) = delete;
    EnumMap(const EnumMap&&) = delete;
    ~EnumMap(){}

    std::optional<T> find(const std::string& key) const {
        auto iter = _map.find(key);
        if (iter == _map.end()){
            return std::nullopt;
        }
        return iter->second;
    }

    const std::string& available() const {
        return _available;
    }

private:
    const Map _map;
    const std::string _available;

    std::string _getAvailable(){
        std::string available = "[";
        for (auto& pair : _map){
            available += pair.first + ", ";
        }
        available.pop_back();
        available.pop_back();
        available += "]";

        return available;
    }

};


} // namespace native::js