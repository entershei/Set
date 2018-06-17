//
// Created by Ira_F on 10.06.2018.
//

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <utility>
#include <cstring>
#include <sstream>
#include "../set.h"
#include "../gtest/gtest.h"
#include <gtest/gtest.h>

using namespace my_Set;

namespace {
    TEST(correctness, empty_set) {
        set<int> x;
        EXPECT_EQ(x.empty(), true);
    }

    TEST(swaps, swap_empty) {
        set<int> x;
        set<int> y;
        x.insert(2);
        swap(x,y);
        EXPECT_FALSE(y.empty());
        swap(x,y);
        EXPECT_FALSE(x.empty());
    }
}