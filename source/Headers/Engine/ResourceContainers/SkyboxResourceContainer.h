#pragma once

#include <glad/glad.h>

class SkyboxResourceContainer {
private:
    const unsigned int VBO{}, VAO{};

public:
    SkyboxResourceContainer(const SkyboxResourceContainer&) = delete;
    SkyboxResourceContainer& operator=(const SkyboxResourceContainer&) = delete;
    SkyboxResourceContainer(SkyboxResourceContainer&&) = delete;
    SkyboxResourceContainer& operator=(SkyboxResourceContainer&&) = delete;

    SkyboxResourceContainer() = delete;

    SkyboxResourceContainer(unsigned int VAOPar, unsigned int VBOPar) noexcept;

    [[nodiscard]] unsigned int getVAO() const noexcept;

    ~SkyboxResourceContainer() noexcept;
};
