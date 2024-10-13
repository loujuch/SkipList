#ifndef _UTIL_HPP__
#define _UTIL_HPP__

#include <gtest/gtest.h>

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

#include "skip_list.hpp"

namespace bit {

template <typename T>
using sl_set = bit::SkipList<T, T, std::_Identity<T>, std::less<T>>;

template <typename K, typename V>
using sl_map = bit::SkipList<K, std::pair<K, V>,
    std::_Select1st<std::pair<K, V>>, std::less<K>>;

struct TestClass {
public:
    int value;
public:
    inline static bool fail = false;

    inline static uint64_t init_ctor = 0;
    inline static uint64_t copy_ctor = 0;
    inline static uint64_t move_ctor = 0;
    inline static uint64_t copy_assign = 0;
    inline static uint64_t move_assign = 0;
    inline static uint64_t dtor = 0;
public:
    TestClass(int value) : value(value) {
        ++init_ctor;
    }

    TestClass(const TestClass &t) : value(t.value) {
        ++copy_ctor;
    }

    TestClass(TestClass &&t) : value(t.value) {
        ++move_ctor;
    }

    ~TestClass() {
        ++dtor;
    }

    TestClass &operator=(const TestClass &t) {
        if (this != &t) {
            ++copy_assign;
            value = t.value;
        }
        return *this;
    }

    TestClass &operator=(TestClass &&t) {
        if (this != &t) {
            value = t.value;
            ++move_assign;
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

    bool operator==(const TestClass &t) const {
        return value == t.value;
    }

    inline static bool check() {
        return (!fail) & (init_ctor + copy_ctor + move_ctor == dtor);
    }

    inline static void reset() {
        init_ctor = 0;
        copy_ctor = 0;
        move_ctor = 0;
        copy_assign = 0;
        move_assign = 0;
        dtor = 0;
        fail = false;
    }

    TestClass() {
        fail = true;
    }
};

using vec_t = std::vector<TestClass>;

class RandomTestVector final {
public:
    friend const vec_t &get_random_vector(std::size_t size);
    friend const vec_t &get_sorted_vector(std::size_t size);
    friend const vec_t &get_unique_vector(std::size_t size);
    friend void reset(std::size_t size);
private:
    static RandomTestVector &getInstance(std::size_t size) {
        static std::unordered_map<std::size_t, RandomTestVector> um;
        if (um.count(size) == 0) {
            um.emplace(size, RandomTestVector(size, 0));
        }
        return um.at(size);
    }

    RandomTestVector(std::size_t n, uint32_t seed) :
        random_vector(n), unique_vector(n) {
        std::default_random_engine dre(seed);
        std::uniform_int_distribution<> uid;

        std::size_t rand_size = n * 0.75;
        std::size_t equal_size = n - rand_size;

        for (std::size_t i = 0; i < rand_size; ++i) {
            random_vector[i] = uid(dre);
        }

        std::copy(random_vector.begin(),
                  random_vector.begin() + equal_size,
                  random_vector.begin() + rand_size);
        std::shuffle(random_vector.begin(), random_vector.end(), std::mt19937(dre()));

        sorted_vector = random_vector;
        std::sort(sorted_vector.begin(), sorted_vector.end());

        auto it = std::unique_copy(sorted_vector.begin(), sorted_vector.end(),
                                   unique_vector.begin());
        unique_vector.resize(it - unique_vector.begin());
    }
private:
    std::vector<TestClass> random_vector;
    std::vector<TestClass> sorted_vector;
    std::vector<TestClass> unique_vector;
};

inline const vec_t &get_random_vector(std::size_t size) {
    return RandomTestVector::getInstance(size).random_vector;
}

inline const vec_t &get_sorted_vector(std::size_t size) {
    return RandomTestVector::getInstance(size).sorted_vector;
}

inline const vec_t &get_unique_vector(std::size_t size) {
    return RandomTestVector::getInstance(size).unique_vector;
}

inline void reset(std::size_t size) {
    static std::unordered_map<std::size_t, int> um;
    RandomTestVector::getInstance(size) = RandomTestVector(size, ++um[size]);
}

}

#endif // _UTIL_HPP__