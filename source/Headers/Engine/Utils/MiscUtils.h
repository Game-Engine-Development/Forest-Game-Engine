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
#include "Headers/Engine/Utils/CommonDeclarations.h"

[[nodiscard]] float mathRound(float value, std::int32_t numberOfDigits) noexcept;

bool isIntegral(char num);

void reverse(unsigned char *start, std::size_t block_size, std::size_t numOfBlocks);

void screenshot(const char *filename, const Window &window);

[[nodiscard]] Component::Transform createModelMatrix(const Component::PosRotationScale &transform) noexcept;

//true = 1, false = -1
std::int8_t boolToSign(bool b);

template<typename T>
bool future_is_ready(const std::future<T>& t);

std::ostream& operator<<(std::ostream &stream, glm::vec3 data);



//implementation:
template<typename T>
bool future_is_ready(const std::future<T>& t) {
    if(!t.valid()) return false;
    return t.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}



struct FilePointerWrapper {
    FilePointerWrapper(const FilePointerWrapper&) = delete;
    FilePointerWrapper& operator=(const FilePointerWrapper&) = delete;
    FilePointerWrapper(FilePointerWrapper&&) = delete;
    FilePointerWrapper& operator=(FilePointerWrapper&&) = delete;

    FilePointerWrapper(const char *filename, const char *modes);

    [[nodiscard]] FILE* getFile() const noexcept;

    ~FilePointerWrapper();

private:
    FILE *const file;
};
