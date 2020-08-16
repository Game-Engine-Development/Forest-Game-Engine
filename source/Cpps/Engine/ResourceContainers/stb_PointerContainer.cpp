#include "../../../../libraries/stb_image.h"

#include "Headers/Engine/ResourceContainers/stb_PointerContainer.h"

void swap(stb_PointerContainer &imageWrapper1, stb_PointerContainer &imageWrapper2) {
    std::swap(imageWrapper1.data, imageWrapper2.data);

    std::swap(imageWrapper1.width, imageWrapper2.width);
    std::swap(imageWrapper1.height, imageWrapper2.height);
    std::swap(imageWrapper1.nrchannels, imageWrapper2.nrchannels);
}

stb_PointerContainer::stb_PointerContainer(unsigned char *const image, int imageWidth, int imageHeight, int imageChannels) noexcept
  : data(image), width(imageWidth), height(imageHeight), nrchannels(imageChannels)
{}

[[nodiscard]] const unsigned char* stb_PointerContainer::getData() const noexcept {
    return data;
}
[[nodiscard]] int stb_PointerContainer::getWidth() const noexcept {
    return width;
}
[[nodiscard]] int stb_PointerContainer::getHeight() const noexcept {
    return height;
}
[[nodiscard]] int stb_PointerContainer::getChannels() const noexcept {
    return nrchannels;
}

stb_PointerContainer::~stb_PointerContainer() {
    stbi_image_free(data);
}
