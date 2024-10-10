#ifndef _UTIL_HPP__
#define _UTIL_HPP__

#include <gtest/gtest.h>

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

#include "../include/skip_list.hpp"

namespace bit {

template <typename T>
using sl_set = bit::SkipList<T, T, std::_Identity<T>, std::less<T>>;

template <typename K, typename V>
using sl_map = bit::SkipList<K, std::pair<K, V>,
    std::_Select1st<std::pair<K, V>>, std::less<K>>;

class TestClass {
    int value;

    // 调用默认构造函数次数
    inline static uint64_t init = 0;
    // 调用拷贝构造或拷贝赋值次数
    inline static uint64_t copy = 0;
    // 调用移动构造或拷贝赋值次数
    inline static uint64_t move = 0;
    // 调用析构
    inline static uint64_t dtor = 0;
public:
    TestClass(int value) : value(value) {
        ++init;
    }

    TestClass(const TestClass &t) : value(t.value) {
        ++copy;
    }

    TestClass(TestClass &&t) : value(t.value) {
        ++move;
    }

    ~TestClass() {
        ++dtor;
    }

    TestClass &operator=(const TestClass &t) {
        if (this != &t) {
            ++copy;
            value = t.value;
        }
        return *this;
    }

    TestClass &operator=(TestClass &&t) {
        if (this != &t) {
            ++move;
            value = t.value;
        }
        return *this;
    }

    void swap(TestClass &t) {
        if (this != &t) {
            std::swap(value, t.value);
        }
    }

    bool operator<(const TestClass &t) const {
        return value < t.value;
    }

    inline bool getValue() const {
        return value;
    }

    inline static uint64_t get_init() {
        return init;
    }

    inline static uint64_t get_copy() {
        return copy;
    }

    inline static uint64_t get_move() {
        return move;
    }

    inline static uint64_t get_dtor() {
        return dtor;
    }

    inline static bool check() {
        return get_init() + get_copy() + get_move() == get_dtor();
    }

    inline static bool reset() {
        if (!check()) {
            return false;
        }
        init = 0;
        copy = 0;
        move = 0;
        dtor = 0;
        return true;
    }
};

using vec_t = std::vector<int>;

class RandomIntVector {
public:
    friend const vec_t &get_random_vector(std::size_t size);
    friend const vec_t &get_sorted_vector(std::size_t size);
    friend const vec_t &get_unique_vector(std::size_t size);
    friend void reset(std::size_t size);
private:
    static RandomIntVector &getInstance(std::size_t size) {
        static std::unordered_map<std::size_t, RandomIntVector> um;
        if (um.count(size) == 0) {
            um.emplace(size, RandomIntVector(size, 0));
        }
        return um.at(size);
    }

    RandomIntVector(int n, int seed) : random_vector(n), unique_vector(n) {
        std::default_random_engine dre(seed);
        std::uniform_int_distribution<> uid;

        sorted_vector = random_vector;
        std::sort(sorted_vector.begin(), sorted_vector.end());

        auto it = std::unique_copy(sorted_vector.begin(), sorted_vector.end(),
                                   unique_vector.begin());
        unique_vector.erase(it, unique_vector.end());
    }
private:
    std::vector<int> random_vector;
    std::vector<int> sorted_vector;
    std::vector<int> unique_vector;
};

const vec_t &get_random_vector(std::size_t size) {
    return RandomIntVector::getInstance(size).random_vector;
}

const vec_t &get_sorted_vector(std::size_t size) {
    return RandomIntVector::getInstance(size).sorted_vector;
}

const vec_t &get_unique_vector(std::size_t size) {
    return RandomIntVector::getInstance(size).unique_vector;
}

void reset(std::size_t size) {
    static std::unordered_map<std::size_t, int> um;
    RandomIntVector::getInstance(size) = RandomIntVector(size, ++um[size]);
}

}

#endif // _UTIL_HPP__