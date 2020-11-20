#pragma once

#include <algorithm>

class stb_PointerContainer {
    friend void swap(stb_PointerContainer &imageWrapper1, stb_PointerContainer &imageWrapper2);

    unsigned char *data = nullptr;
    int width{}, height{}, nrchannels{};

public:
    stb_PointerContainer(const stb_PointerContainer&) = delete;
    stb_PointerContainer& operator=(const stb_PointerContainer&) = delete;

    stb_PointerContainer(stb_PointerContainer &&oldImageWrapper) noexcept
    : data(oldImageWrapper.data), width(oldImageWrapper.width),
      height(oldImageWrapper.height), nrchannels(oldImageWrapper.nrchannels)
    {
        oldImageWrapper.data = nullptr;
        oldImageWrapper.width = 0;
        oldImageWrapper.height = 0;
        oldImageWrapper.nrchannels = 0;
    }
    stb_PointerContainer& operator=(stb_PointerContainer &&oldImageWrapper) noexcept {
        stb_PointerContainer move(std::move(oldImageWrapper));
        swap(*this, move);
        return *this;
    }

    stb_PointerContainer() = delete;

    explicit stb_PointerContainer(unsigned char *image, int imageWidth, int imageHeight, int imageChannels) noexcept;

    [[nodiscard]] const unsigned char* getData() const noexcept;
    [[nodiscard]] int getWidth() const noexcept;
    [[nodiscard]] int getHeight() const noexcept;
    [[nodiscard]] int getChannels() const noexcept;

    ~stb_PointerContainer();
};
