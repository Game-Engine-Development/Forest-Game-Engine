#pragma once

#include <algorithm>
#include <iostream>
#include <cmath>
#include <memory>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/vec3.hpp>

#include "Headers/Engine/IO/Window.h"

float mathRound(float value);

bool isIntegral(char num);

void reverse(unsigned char *start, std::size_t block_size, std::size_t numOfBlocks);

void screenshot(const char *filename, const Window &window);
