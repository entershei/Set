#include <iostream>
#include "set.h"
#include "../gtest/gtest.h"
#include <gtest/gtest.h>

using namespace my_Set;

namespace {
    TEST(correctness, empty_set) {
        set<int> x;
    }

//    TEST(swaps, swap_empty) {
//        set<int> x;
//        set<int> y;
//        x.insert(2);
//        swap(x,y);
//        EXPECT_FALSE(y.empty());
//        swap(x,y);
//        EXPECT_FALSE(x.empty());
//    }
}

int main() {
    
    return 0;
}