#pragma once

#include <algorithm>
#include <iostream>
#include <cmath>
#include <memory>

#include <future>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/vec3.hpp>

#include "Headers/Engine/IO/Window.h"

float mathRound(float value);

bool isIntegral(char num);

void reverse(unsigned char *start, std::size_t block_size, std::size_t numOfBlocks);

void screenshot(const char *filename, const Window &window);

template<typename T>
bool future_is_ready(std::future<T>& t);





//implementation:
template<typename T>
bool future_is_ready(std::future<T>& t) {
    return t.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}
