#pragma once

#include <iostream>
#include <cstddef>
#include <array>
#include <vector>
#include <cassert>
#include <memory>
#include <cstdio>
#include <cinttypes>
#include <type_traits>
#include <initializer_list>
#include <memory_resource>

#include "Headers/Engine/Utils/CollisionDetection/CollisionDetection.h"

static int count = 0;

struct Foo {
    std::int32_t fooVar;
    Foo() {
        std::printf("Default construction of Foo %d\n", count);
        ++count;
    }
};

//WARNING: Performance for SmallVector is very situational and should only be used if you know what you are doing
//NOTE: pmr uses virtual function calls
template <typename T, std::size_t N = 128u>
class SmallVector {
    alignas(T) std::byte smallBuffer[sizeof(T)*N];
    std::pmr::monotonic_buffer_resource bufferResource{smallBuffer, sizeof(smallBuffer)};
public:
    std::pmr::vector<T> dynamicArray{&bufferResource};
};



template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
class FreeList {

};


