#include <gtest/gtest.h>

#include "util.hpp"

TEST(ctor, case0) {
    auto rv = bit::get_random_vector(1024);
    auto uv = bit::get_unique_vector(1024);
    bit::TestClass::reset();

    {
        bit::sl_set<bit::TestClass> sls(rv.begin(), rv.end());
        ASSERT_EQ(sls.size(), rv.size());

        bit::sl_set<bit::TestClass> other(sls);

        ASSERT_EQ(sls.size(), rv.size());
        ASSERT_EQ(other.size(), rv.size());

        auto p = sls.begin();
        auto q = other.begin();
        while (p != sls.end() && q != other.end()) {
            ASSERT_EQ(*p++, *q++);
        }
    }

    ASSERT_EQ(0, bit::TestClass::init_ctor);
    ASSERT_EQ(2 * rv.size(), bit::TestClass::copy_ctor);
    ASSERT_EQ(0, bit::TestClass::move_ctor);
    ASSERT_EQ(0, bit::TestClass::copy_assign);
    ASSERT_EQ(0, bit::TestClass::move_assign);

    ASSERT_TRUE(bit::TestClass::check());
}

TEST(ctor, case1) {
    auto rv = bit::get_random_vector(1024);
    auto sv = bit::get_sorted_vector(1024);
    bit::TestClass::reset();

    {
        bit::sl_set<bit::TestClass> sls(rv.begin(), rv.end());
        ASSERT_EQ(sls.size(), rv.size());

        bit::sl_set<bit::TestClass> other(std::move(sls));

        ASSERT_TRUE(sls.empty());
        ASSERT_EQ(other.size(), rv.size());

        auto p = sv.begin();
        auto q = other.begin();
        while (p != sv.end() && q != other.end()) {
            ASSERT_EQ(*p++, *q++);
        }
    }

    ASSERT_EQ(0, bit::TestClass::init_ctor);
    ASSERT_EQ(rv.size(), bit::TestClass::copy_ctor);
    ASSERT_EQ(0, bit::TestClass::move_ctor);
    ASSERT_EQ(0, bit::TestClass::copy_assign);
    ASSERT_EQ(0, bit::TestClass::move_assign);

    ASSERT_TRUE(bit::TestClass::check());
}

TEST(ctor, case2) {
    auto rv = bit::get_random_vector(1024);
    auto sv = bit::get_sorted_vector(1024);
    auto uv = bit::get_unique_vector(1024);
    bit::TestClass::reset();

    {
        bit::sl_set<bit::TestClass> sls;
        bit::sl_set<bit::TestClass> other;

        std::unordered_set<void *>ss;

        for (const auto &i : rv) {
            sls.insert_equal(i);
            other.insert_unique(i);
        }

        ASSERT_EQ(sls.size(), rv.size());
        ASSERT_EQ(other.size(), uv.size());

        other = sls;

        ASSERT_EQ(sls.size(), rv.size());
        ASSERT_EQ(other.size(), rv.size());

        auto p = sls.begin();
        auto q = other.begin();
        while (p != sls.end() && q != other.end()) {
            ASSERT_EQ(*p++, *q++);
        }
    }

    ASSERT_EQ(0, bit::TestClass::init_ctor);
    ASSERT_EQ(2 * rv.size() + uv.size(), bit::TestClass::copy_ctor);
    ASSERT_EQ(0, bit::TestClass::move_ctor);
    ASSERT_EQ(0, bit::TestClass::copy_assign);
    ASSERT_EQ(0, bit::TestClass::move_assign);

    ASSERT_TRUE(bit::TestClass::check());
}

TEST(ctor, case3) {
    auto rv = bit::get_random_vector(1024);
    auto sv = bit::get_sorted_vector(1024);
    auto uv = bit::get_unique_vector(1024);
    bit::TestClass::reset();

    {
        bit::sl_set<bit::TestClass> sls;
        bit::sl_set<bit::TestClass> other;

        std::unordered_set<void *>ss;

        for (const auto &i : rv) {
            sls.insert_equal(i);
            other.insert_unique(i);
        }

        ASSERT_EQ(sls.size(), rv.size());
        ASSERT_EQ(other.size(), uv.size());

        other = std::move(sls);

        ASSERT_TRUE(sls.empty());
        ASSERT_EQ(other.size(), rv.size());

        auto p = sv.begin();
        auto q = other.begin();
        while (p != sv.end() && q != other.end()) {
            ASSERT_EQ(*p++, *q++);
        }
    }

    ASSERT_EQ(0, bit::TestClass::init_ctor);
    ASSERT_EQ(rv.size() + uv.size(), bit::TestClass::copy_ctor);
    ASSERT_EQ(0, bit::TestClass::move_ctor);
    ASSERT_EQ(0, bit::TestClass::copy_assign);
    ASSERT_EQ(0, bit::TestClass::move_assign);

    ASSERT_TRUE(bit::TestClass::check());
}

TEST(ctor, case4) {
    auto rv = bit::get_random_vector(1024);
    auto sv = bit::get_sorted_vector(1024);
    auto uv = bit::get_unique_vector(1024);
    bit::TestClass::reset();

    {
        bit::sl_set<bit::TestClass> sls(rv.begin(), rv.end());

        ASSERT_EQ(sls.size(), rv.size());

        auto p = sls.begin();
        auto q = sv.begin();
        while (p != sls.end() && q != sv.end()) {
            ASSERT_EQ(*p++, *q++);
        }
    }

    ASSERT_EQ(0, bit::TestClass::init_ctor);
    ASSERT_EQ(rv.size(), bit::TestClass::copy_ctor);
    ASSERT_EQ(0, bit::TestClass::move_ctor);
    ASSERT_EQ(0, bit::TestClass::copy_assign);
    ASSERT_EQ(0, bit::TestClass::move_assign);

    ASSERT_TRUE(bit::TestClass::check());
}