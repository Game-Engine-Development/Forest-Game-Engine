#pragma once

#include <iostream>
#include <cstddef>
#include <array>
#include <vector>
#include <cassert>
#include <memory>

#include "Headers/Engine/Utils/CollisionDetection/CollisionDetection.h"




template <std::size_t NumberOfChildren>
class SpacePartitionTree {
    const std::size_t treeLevel = 0;
    const std::size_t capacity = 8;
    const BoundingBox boundary{};

    std::vector<Coordinate> points{};

    std::array<std::unique_ptr<SpacePartitionTree>, NumberOfChildren> children{}; //by default are nullptr

public:
    explicit SpacePartitionTree(const BoundingBox &boundary)
            : boundary(boundary)
    {}

    SpacePartitionTree(const std::size_t treeLevel, const std::size_t capacity, const BoundingBox &boundary)
            : treeLevel(treeLevel), capacity(capacity), boundary(boundary)
    {}

    bool insert(const Coordinate p) {
        if (!boundary.containsPoint(p)) return false; // object cannot be added

        if (points.size() < capacity && children[0] == nullptr) {
            points.push_back(p);
            return true;
        }

        // Otherwise, subdivide and then add the point to whichever node will accept it
        if (children[0] == nullptr) subdivide();
        //We have to add the points/data contained into this quad array to the new quads if we only want
        //the last node to hold the data

        for(const auto &child : children) {
            if(child->insert(p)) return true;
        }

        // Otherwise, the point cannot be inserted for some unknown reason (this should never happen)
        assert(false);
        return false;
    }

    void subdivide() {
        const auto getNewBoundingBox = [&](const int i) {
            const LengthVec3 newHalfWidths = boundary.halfWidths/2.0f;

            const auto getPartition = [&] (const std::array<bool, 3> negative) {
                LengthVec3 halfWidths{};
                for(int index = 0; index < 3; ++index) {
                    halfWidths[index] = (negative[index]) ? -newHalfWidths[index] : newHalfWidths[index];
                }
                return BoundingBox{Coordinate{boundary.center + halfWidths}, newHalfWidths};
            };

            switch(i) {
                case 0:
                    return getPartition({true, false, true});
                case 1:
                    return getPartition({true, true, true});

                case 2:
                    return getPartition({false, false, true});
                case 3:
                    return getPartition({false, true, true});


                case 4:
                    return getPartition({true, false, false});
                case 5:
                    return getPartition({true, true, false});

                case 6:
                    return getPartition({false, false, false});
                case 7:
                    return getPartition({false, true, false});

                default:
                    std::cerr << "Error: Unreachable code\n";
                    assert(false);
            }
        };

        for(int i = 0; i < children.size(); ++i) {
            children[i] = std::make_unique<SpacePartitionTree>(capacity, getNewBoundingBox(i), treeLevel+1);
        }
    }


    // Find all points that appear within a range
    [[nodiscard]] std::vector<Coordinate> queryRange(const BoundingBox range) const {
        // Prepare an array of results
        std::vector<Coordinate> pointsInRange;

        // Automatically abort if the range does not intersect this quad
        if (!boundary.intersectsAABB(range)) {
            return pointsInRange; // empty list
        }

        // Check objects at this quad level
        for (const auto point : points) {
            if (range.containsPoint(point)) {
                pointsInRange.push_back(point);
            }
        }

        // Terminate here, if there are no children
        if (children[0] == nullptr) return pointsInRange;

        // Otherwise, add the points from the children
        for(const auto &child : children) {
            if(child) {
                const auto childVector = child->queryRange(range);
                pointsInRange.insert(std::end(pointsInRange), std::begin(childVector), std::end(childVector));
            }
        }

        return pointsInRange;
    }

    friend std::ostream& operator<<(std::ostream &stream, const SpacePartitionTree<NumberOfChildren> &octtree) {
        static std::size_t lastTreeLevel = 0;
        if (octtree.treeLevel != lastTreeLevel) {
            stream << '\n';
            lastTreeLevel = octtree.treeLevel;
        }
        stream << "size: " << octtree.points.size() << ", treeLevel: " << octtree.treeLevel << '\n';
        for (const auto &child : octtree.children) {
            if (child) stream << *child;
        }
        return stream;
    }

};
