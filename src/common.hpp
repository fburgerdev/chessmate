/*
    This header file was automatically created by 'cpp-library',
    it is not meant to be modified.
    For more infos, see https://github.com/fburgerdev/cpp-library
*/
#pragma once
#include <ranges> // stdr
#include <memory> // address, int, uint
#include <string> // string
#include <string_view> // string_view
#include <array> // Array
#include <vector> // List
#include <stack> // Stack
#include <queue> // Queue
#include <deque> // Deque
#include <unordered_set> // Set 
#include <map> // Map
#include <set> // HashSet
#include <unordered_map> // HashMap
#include <functional> // function
#include <optional> // Opt
#include <iostream> // cout, endl

namespace Chessmate {
    // ranges
    namespace stdr = std::ranges;

    // types
    // types :: address
    using address = std::size_t;
    // types :: int
    using int8 = std::int8_t;
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using int64 = std::int64_t;
    // types :: uint
    using uint8 = std::uint8_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;
    using uint64 = std::uint64_t;
    // types :: string
    using string = std::string;
    using string_view = std::string_view;

    // container
    // container :: sequential
    template<typename T, address N>
    using Array = std::array<T, N>;
    template<typename T>
    using List = std::vector<T>;
    template<typename T>
    using Stack = std::stack<T>;
    template<typename T>
    using Queue = std::queue<T>;
    template<typename T>
    using Deque = std::deque<T>;
    // container :: tree
    template<typename T>
    using Set = std::set<T>;
    template<typename Key, typename Value>
    using Map = std::map<Key, Value>;
    // container :: hash
    template<typename T>
    using HashSet = std::unordered_set<T>;
    template<typename Key, typename Value>
    using HashMap = std::unordered_map<Key, Value>;

    // move semantics
    using std::forward;
    using std::move;

    // smart pointers
    using std::make_unique;
    using std::make_shared;
    using std::unique_ptr;
    using std::shared_ptr;
    using std::weak_ptr;

    // function
    using std::function;

    // templates
    template<typename T>
    using Opt = std::optional<T>;

    // debug
    using std::cout;
    using std::endl;
}