#include "Headers/Engine/Utils/MiscUtils.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../../libraries/stb_image_write.h"

[[nodiscard]] float mathRound(const float value) noexcept {
    return std::round(value * 1000.0f) / 1000.0f;
}

bool isIntegral(const char num) {
    static constexpr int ZERO = 48;
    static constexpr int NINE = 57;

    //has an ascii code in between (and including) the literals 0 & 9
    return ZERO <= num && num <= NINE;
}

void reverse(unsigned char *const start, const std::size_t block_size, const std::size_t numOfBlocks) {
    unsigned char *const end = (start+(block_size*numOfBlocks))-1; //-1 because arrays are 0 indexed
    for(int x1 = 0; x1 < numOfBlocks/2; ++x1) {
        for(int x2 = 0; x2 < block_size; ++x2) {
            std::swap(start[x1*block_size + x2], end[-(x1*block_size) - (block_size-1) + x2]);
        }
    }
}

void screenshot(const char *const filename, const Window &window) {
    constexpr int NUM_OF_CHANNELS = 4;
    const auto pixels = std::make_unique<GLubyte[]>(NUM_OF_CHANNELS * static_cast<int>(window.getWidth()) * static_cast<int>(window.getHeight()));

    glReadPixels(0, 0, static_cast<int>(window.getWidth()), static_cast<int>(window.getHeight()), GL_RGBA, GL_UNSIGNED_BYTE, pixels.get());

    for(int i = 0; i < static_cast<int>(window.getHeight()); ++i) {
        reverse(pixels.get(), NUM_OF_CHANNELS, static_cast<int>(window.getWidth()));
    }
    reverse(pixels.get(), NUM_OF_CHANNELS * static_cast<int>(window.getWidth()), static_cast<int>(window.getHeight()));

    stbi_write_jpg(filename, static_cast<int>(window.getWidth()), static_cast<int>(window.getHeight()), NUM_OF_CHANNELS, pixels.get(), 100);
}

[[nodiscard]] Component::Transform createModelMatrix(const Component::PosRotationScale &transform) noexcept {
    Component::Transform modelMatrix = Component::Transform (1.0f);
    modelMatrix = glm::translate(modelMatrix, transform.getPos());
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.getRotation().x), glm::vec3(1,0,0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.getRotation().y), glm::vec3(0,1,0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.getRotation().z), glm::vec3(0,0,1));
    modelMatrix = glm::scale(modelMatrix, transform.getScale());
    return modelMatrix;
}

std::ostream& operator<<(std::ostream &stream, const glm::vec3 data) {
    stream << '{' << data.x << ", " << data.y << ", " << data.z << '}';
    return stream;
}
