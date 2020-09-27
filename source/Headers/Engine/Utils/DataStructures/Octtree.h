#pragma once

#include <iostream>
#include <cstddef>
#include <array>
#include <vector>
#include <cassert>
#include <memory>

#include <glm/glm.hpp>


using Coordinate = glm::vec3;

struct BoundingBox {
    Coordinate center;
    std::array<float, 3> halfWidths;

    BoundingBox(Coordinate point, std::array<float, 3> halfWidths);


    [[nodiscard]] bool containsPoint(Coordinate point) const noexcept;

    [[nodiscard]] bool intersectsAABB(const BoundingBox &other) const noexcept;
};

class Octtree {
    const std::size_t treeLevel;
    const std::size_t capacity;
    const BoundingBox boundary;

    std::vector<Coordinate> points;

    std::array<std::unique_ptr<Octtree>, 8> children{}; //by default are nullptr

public:
    explicit Octtree(const BoundingBox &boundary);

    Octtree(std::size_t capacity, const BoundingBox &boundary, std::size_t treeLevel);


    bool insert(Coordinate p);

    void subdivide();

    // Find all points that appear within a range
    [[nodiscard]] std::vector<Coordinate> queryRange(BoundingBox range) const;


    friend std::ostream& operator<< (std::ostream &stream, const Octtree &octtree);
};
