#include <gtest/gtest.h>

#include "util.hpp"

TEST(lower_bound, case0) {
    auto rv = bit::get_random_vector(1024);
    auto uv = bit::get_unique_vector(1024);
    bit::TestClass::reset();

    {
        bit::sl_set<bit::TestClass> sls;

        std::unordered_set<void *> ss;

        for (const auto &i : rv) {
            auto it = sls.insert_equal(i);
            ASSERT_EQ(ss.count(it.base()), 0);
            ASSERT_EQ(i, *it);
            ss.emplace(it.base());
        }

        ASSERT_EQ(sls.size(), rv.size());

        ASSERT_EQ(sls.begin(), sls.lower_bound(bit::TestClass(INT32_MIN)));
        ASSERT_EQ(sls.end(), sls.lower_bound(bit::TestClass(INT32_MAX)));

        for (const auto &i : uv) {
            for (int j = -1; j <= 1; ++j) {
                auto tmpv = bit::TestClass(i.value + j);
                auto tmp = sls.lower_bound(tmpv);
                if (tmpv.value <= sls.front().value) {
                    ASSERT_EQ(tmp, sls.begin());
                } else if (tmpv.value > sls.back().value) {
                    ASSERT_EQ(tmp, sls.end());
                } else {
                    ASSERT_LE(tmpv.value, tmp->value);
                    ASSERT_GT(tmpv.value, (--tmp)->value);
                }
            }
        }
    }
}

TEST(upper_bound, case0) {
    auto rv = bit::get_random_vector(1024);
    auto uv = bit::get_unique_vector(1024);
    bit::TestClass::reset();

    {
        bit::sl_set<bit::TestClass> sls;

        std::unordered_set<void *> ss;

        for (const auto &i : rv) {
            auto it = sls.insert_equal(i);
            ASSERT_EQ(ss.count(it.base()), 0);
            ASSERT_EQ(i, *it);
            ss.emplace(it.base());
        }

        ASSERT_EQ(sls.size(), rv.size());

        ASSERT_EQ(sls.begin(), sls.lower_bound(bit::TestClass(INT32_MIN)));
        ASSERT_EQ(sls.end(), sls.lower_bound(bit::TestClass(INT32_MAX)));

        for (const auto &i : uv) {
            for (int j = -1; j <= 1; ++j) {
                auto tmpv = bit::TestClass(i.value + j);
                auto tmp = sls.upper_bound(tmpv);
                if (tmpv.value < sls.front().value) {
                    ASSERT_EQ(tmp, sls.begin());
                } else if (tmpv.value >= sls.back().value) {
                    ASSERT_EQ(tmp, sls.end());
                } else {
                    ASSERT_LT(tmpv.value, tmp->value);
                    ASSERT_GE(tmpv.value, (--tmp)->value);
                }
            }
        }
    }
}

TEST(find, case0) {
    auto rv = bit::get_random_vector(1024);
    auto uv = bit::get_unique_vector(1024);
    bit::TestClass::reset();

    {
        bit::sl_set<bit::TestClass> sls;

        std::unordered_set<void *> ss;

        for (const auto &i : rv) {
            auto it = sls.insert_equal(i);
            ASSERT_EQ(ss.count(it.base()), 0);
            ASSERT_EQ(i, *it);
            ss.emplace(it.base());
        }

        ASSERT_EQ(sls.size(), rv.size());

        if (sls.front().value != INT32_MIN) {
            ASSERT_EQ(sls.end(), sls.find(bit::TestClass(INT32_MIN)));
        }

        if (sls.back().value != INT32_MAX) {
            ASSERT_EQ(sls.end(), sls.find(bit::TestClass(INT32_MAX)));
        }

        for (const auto &i : uv) {
            auto tmp = sls.find(bit::TestClass(i.value));
            ASSERT_NE(tmp, sls.end());
            ASSERT_EQ(*tmp, i);
        }
    }
}