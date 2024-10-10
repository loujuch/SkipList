#include <gtest/gtest.h>

#include "util.hpp"

TEST(insert_equal, case0) {
    // 对内建类型确认能够完成输入并自动排序，测试使用左值传递
    bit::sl_set<int> sls;
    auto rv = bit::get_random_vector(1024);
    auto sv = bit::get_sorted_vector(1024);

    std::unordered_set<void *>ss;

    for (int i : rv) {
        // 确认返回的迭代器有效
        auto it = sls.emplace_equal(i);
        ASSERT_EQ(ss.count(it.base()), 0);
        ASSERT_EQ(i, *it);
        ss.emplace(it.base());
    }

    ASSERT_EQ(sls.size(), rv.size());

    // 确认能够自动排序
    auto slit = sls.begin();
    auto svit = sv.begin();
    while (slit != sls.end() && svit != rv.end()) {
        ASSERT_EQ(*slit++, *svit++);
    }
}

TEST(insert_equal, case1) {
    // 对内建类型确认能够完成输入并自动排序，测试使用左值传递
    bit::sl_set<int> sls;
    auto rv = bit::get_random_vector(1024);
    auto sv = bit::get_sorted_vector(1024);

    std::unordered_set<void *>ss;

    for (int i : rv) {
        // 确认返回的迭代器有效
        auto it = sls.emplace_equal(i);
        ASSERT_EQ(ss.count(it.base()), 0);
        ASSERT_EQ(i, *it);
    }

    ASSERT_EQ(sls.size(), rv.size());

    // 确认能够自动排序
    auto slit = sls.begin();
    auto svit = sv.begin();
    while (slit != sls.end() && svit != rv.end()) {
        ASSERT_EQ(*slit++, *svit++);
    }
}