#include <gtest/gtest.h>

#include "util.hpp"

TEST(erase, case0) {
    auto rv = bit::get_random_vector(1024);
    auto uv = bit::get_unique_vector(1024);
    bit::TestClass::reset();

    {
        bit::sl_set<bit::TestClass> sls(rv.begin(), rv.end());
        ASSERT_EQ(sls.size(), rv.size());

        auto it = sls.begin();
        while (it != sls.end()) {
            it = sls.erase(it);
        }
        ASSERT_EQ(sls.end(), it);
        ASSERT_TRUE(sls.empty());
    }
}

TEST(erase, case1) {
    auto rv = bit::get_random_vector(1024);
    auto uv = bit::get_unique_vector(1024);
    bit::TestClass::reset();

    {
        bit::sl_set<bit::TestClass> sls(rv.begin(), rv.end());
        ASSERT_EQ(sls.size(), rv.size());

        auto it = sls.erase(sls.begin(), sls.end());
        ASSERT_EQ(sls.end(), it);
        ASSERT_TRUE(sls.empty());
    }
}

TEST(erase, case2) {
    auto rv = bit::get_random_vector(1024);
    auto uv = bit::get_unique_vector(1024);
    bit::TestClass::reset();

    {
        bit::sl_set<bit::TestClass> sls(rv.begin(), rv.end());
        ASSERT_EQ(sls.size(), rv.size());

        bit::vec_t tmp(uv);
        std::default_random_engine dre(0);
        std::shuffle(tmp.begin(), tmp.end(), std::mt19937(dre()));

        std::size_t p = tmp.size() / 2;
        for (auto i = 0; i < p; ++i) {
            sls.erase(bit::TestClass(uv[i]));
        }

        for (auto i = 0; i < p; ++i) {
            ASSERT_FALSE(sls.contain(bit::TestClass(uv[i])));
        }

        for (auto i = p; i < uv.size(); ++i) {
            ASSERT_TRUE(sls.contain(bit::TestClass(uv[i])));
        }
    }
}

TEST(count, case0) {
    auto rv = bit::get_random_vector(1024);
    auto uv = bit::get_unique_vector(1024);
    bit::TestClass::reset();

    {
        bit::sl_set<bit::TestClass> sls(rv.begin(), rv.end());
        ASSERT_EQ(sls.size(), rv.size());

        for (const auto &i : uv) {
            ASSERT_EQ(sls.count(i),
                      std::distance(sls.lower_bound(i), sls.upper_bound(i)));
        }
    }
}

TEST(unique, case0) {
    auto rv = bit::get_random_vector(1024);
    auto sv = bit::get_sorted_vector(1024);
    auto uv = bit::get_unique_vector(1024);
    bit::TestClass::reset();

    {
        bit::sl_set<bit::TestClass> sls(rv.begin(), rv.end());
        ASSERT_EQ(sls.size(), rv.size());
        sls.unique();

        ASSERT_EQ(sls.size(), uv.size());

        auto is = sls.begin();
        auto us = uv.begin();
        while (is != sls.end() && us != uv.end()) {
            ASSERT_EQ(*is++, *us++);
        }
    }
}

TEST(clear, case0) {
    auto rv = bit::get_random_vector(1024);
    auto sv = bit::get_sorted_vector(1024);
    auto uv = bit::get_unique_vector(1024);
    bit::TestClass::reset();

    {
        bit::sl_set<bit::TestClass> sls(rv.begin(), rv.end());
        ASSERT_EQ(sls.size(), rv.size());
        sls.clear();

        ASSERT_TRUE(sls.empty());
    }
}