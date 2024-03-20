#pragma once
#include <memory> // address, int, uint
#include <string> // string
#include <string_view> // string_view
#include <array> // Array
#include <vector> // List
#include <stack> // Stack
#include <queue> // Queue
#include <unordered_set> // Set 
#include <set> // OrderedSet
#include <unordered_map> // Map
#include <map> // OrderedMap

namespace Chessmate {
    // Types
    // Types :: address
    using address = std::size_t;
    // Types :: int
    using int8 = std::int8_t;
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using int64 = std::int64_t;
    // Types :: uint
    using uint8 = std::uint8_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;
    using uint64 = std::uint64_t;
    // Types :: string
    using string = std::string;
    // Types :: string_view
    using string_view = std::string_view;
    // Structures
    template<typename T, address N>
    using Array = std::array<T, N>;
    template<typename T>
    using List = std::vector<T>;
    template<typename T>
    using Stack = std::stack<T>;
    template<typename T>
    using Queue = std::queue<T>;
    template<typename T>
    using Set = std::unordered_set<T>;
    template<typename T>
    using OrderedSet = std::set<T>;
    template<typename Key, typename Value>
    using Map = std::unordered_map<Key, Value>;
    template<typename Key, typename Value>
    using OrderedMap = std::map<Key, Value>;
}