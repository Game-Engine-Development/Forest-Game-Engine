#pragma once

#include <iostream>
#include <cstddef>
#include <array>
#include <vector>
#include <memory>
#include <cstdio>
#include <cinttypes>
#include <type_traits>
#include <optional>
#include <memory_resource>

#include "Headers/Engine/Utils/CommonDeclarations.h"
#include "Headers/Engine/Utils/CollisionDetection/CollisionDetection.h"

namespace DataStructures {
    //WARNING: Performance for SmallVector is very situational and should only be used if you know what you are doing
    //NOTE: pmr uses virtual function calls
    template<typename T, std::size_t N = 128u>
    class SmallVector {
        alignas(T) std::byte smallBuffer[sizeof(T) * N];
        std::pmr::monotonic_buffer_resource bufferResource{smallBuffer, sizeof(smallBuffer)};
    public:
        std::pmr::vector<T> dynamicArray{&bufferResource};
    };


    //This is very expensive to construct and does many heap allocations, use for mostly static data like terrain
    //@todo constrain template with concepts
    class Octree {
        static constexpr std::size_t CAPACITY = 8;

        using Node = std::uint32_t;

        //top bit (most significant bit) = is empty
        //second bit = is a node

        //for nodes: first bit being 1 and the second bit being 0 means it is an empty leaf and the value of the integer is unspecified,
        //           the first bit being 0 and the second bit being 0 means it contains data (is a filled leaf) and the number is an index into `leaves`,
        //           and the first bit being 0 and the second bit being 1 means it is a parent node and the number is an index into `nodes`
        std::vector<Node> nodes;

        std::vector<Coordinate> leaves;

        const BoundingBox rootNode;

        [[nodiscard]] constexpr static std::array<BoundingBox, CAPACITY> getChildAABBs(const BoundingBox parentAABB) {
            //glm::vec3 positive = bitwise index //this is to determine the "quadrant" that the node is in
            //0 = 0,0,0 positives (all negative)
            //1 = 0,0,1 positives (negative x and y, positive z)
            //2 = 0,1,0 positives (negative x, positive y, negative z)
            //3 = 0,1,1 positives (negative x, positive y and z)
            //4 = 1,0,0 positives (positive x, negative y and z)
            //5 = 1,0,1 positives (positive x, negative y, positive z)
            //6 = 1,1,0 positives (positive x and y, negative z)
            //7 = 1,1,1 positives (all positive)
            const auto newHalfWidths = parentAABB.halfWidths / 2.f;
            std::array<BoundingBox, CAPACITY> childAABBs{};
            for (std::uint32_t i = 0; i < childAABBs.size(); ++i) {
                childAABBs.at(i) = BoundingBox{.center=parentAABB.center +
                                                       (newHalfWidths * glm::vec3{boolToSign(i & 0x04u),
                                                                                  boolToSign(i & 0x02u),
                                                                                  boolToSign(i & 0x01u)}),
                                               .halfWidths=newHalfWidths};
            }
            return childAABBs;
        }

    public:
        explicit Octree(const BoundingBox box) : Octree(box, {}) {}

        Octree(const BoundingBox box, const std::vector<Coordinate> &data)
                : rootNode(box) {

            nodes.resize(CAPACITY); //start with a capacity of CAPACITY(8) for first level

            for(std::uint32_t &node : nodes) {
                node |= static_cast<std::uint32_t>(0b10u << 30u);
            }

            insert(data);
        }

        void insert(const Coordinate data) {
            insert(std::vector<Coordinate>{data});
        }

        void insert(const std::vector<Coordinate> &data) {
            //@todo make this filling process better
            std::stack<Coordinate> toProcess;
            for(const auto e : data) {
                toProcess.push(e);
            }

            std::array<BoundingBox, CAPACITY> currentChildAABBs = getChildAABBs(rootNode);
            std::int32_t currentIndex = 0;

            while(!toProcess.empty()) {
                const auto currentData = toProcess.top(); //select the node that needs to be processed

                if(!rootNode.containsPoint(currentData)) {
                    std::cerr << "skipping point " << currentData << ", out of range {center: " << rootNode.center << ", halfWidths: " << rootNode.halfWidths << "} of root node\n";
                    toProcess.pop();
                } else {
                    for (int i = 0; i < currentChildAABBs.size(); ++i) {
                        if (currentChildAABBs.at(i).containsPoint(currentData)) {
                            if (nodes.at(currentIndex + i) >> 31u) {
                                //empty leaf, can just insert currentData
                                leaves.push_back(currentData);
                                nodes.at(currentIndex + i) = static_cast<std::uint32_t>(leaves.size());

                                toProcess.pop(); //remove the data from the toProcess list

                                //reset the index and aabb to root for the next data
                                currentChildAABBs = getChildAABBs(rootNode);
                                currentIndex = 0;
                            } else if (!(nodes.at(currentIndex + i) >> 30u)) {
                                const auto dataInTree = leaves.at((nodes.at(currentIndex + i) & (~static_cast<std::uint32_t>(0b11u << 30u))) - 1u);
                                if((mathRound(dataInTree.x, 5) == mathRound(currentData.x, 5)) && (mathRound(dataInTree.y, 5) == mathRound(currentData.y, 5)) && (mathRound(dataInTree.z, 5) == mathRound(currentData.z, 5))) {
                                    //if the same value already exists in the tree
                                    std::cerr << "skipping point " << currentData << ", duplicate\n";
                                    toProcess.pop();

                                    //reset the index and aabb to root for the next data
                                    currentChildAABBs = getChildAABBs(rootNode);
                                    currentIndex = 0;
                                } else {
                                    //filled leaf, will need to insert this leaf, resize, and reinsert currentData
                                    toProcess.push(dataInTree);

                                    //elements are default constructed to 0
                                    nodes.resize(nodes.size() + CAPACITY);
                                    for(int j = nodes.size() - CAPACITY; j < nodes.size(); ++j) {
                                        nodes.at(j) |= static_cast<std::uint32_t>(0b10u << 30u);
                                    }

                                    nodes.at(currentIndex + i) = static_cast<std::uint32_t>(nodes.size() - CAPACITY) | static_cast<std::uint32_t>(0b01u << 30u);

                                    //move the index and AABBs to new children
                                    currentChildAABBs = getChildAABBs(currentChildAABBs.at(i));
                                    currentIndex = nodes.size() - CAPACITY;
                                }
                            } else {
                                //node, just continue going down by updating the index to the start of the next child nodes
                                currentChildAABBs = getChildAABBs(currentChildAABBs.at(i));
                                currentIndex = nodes.at(currentIndex + i) & (~static_cast<std::uint32_t>(0b11u << 30u));
                            }

                            break; //AABB has been found, stop iterating through them
                        }
                    }
                }
            }
        }

        [[nodiscard]] bool isInTree(const Coordinate coordinate) {
            if(!rootNode.containsPoint(coordinate)) {
                return false;
            }

            struct NodeData {
                std::int32_t index;
                BoundingBox aabb;
            };

            std::array<BoundingBox, CAPACITY> currentChildAABBs = getChildAABBs(rootNode);

            std::stack<NodeData> indexesToProcess;
            for(std::int32_t i = Octree::CAPACITY-1; i >= 0; --i) {
                indexesToProcess.push(NodeData{i, currentChildAABBs.at(i)});
            }

            std::int32_t currentIndex; //tracks first element of the 8 nodes
            BoundingBox currentBoundingBox;
            while(!indexesToProcess.empty()) {
                currentIndex = indexesToProcess.top().index;
                currentBoundingBox = indexesToProcess.top().aabb;

                if(nodes.at(currentIndex) >> 31u) {
                    //empty leaf, do nothing
                    indexesToProcess.pop();
                } else if(!((nodes.at(currentIndex) >> 30u) & 1u)) {
                    //filled leaf, check if the point is the one we are looking for
                    const auto data = leaves.at((nodes.at(currentIndex) & (~static_cast<std::uint32_t>(0b11u << 30u))) - 1u);
                    if((mathRound(coordinate.x, 5) == mathRound(data.x, 5)) && (mathRound(coordinate.y, 5) == mathRound(data.y, 5)) && (mathRound(coordinate.z, 5) == mathRound(data.z, 5))) {
                        return true;
                    }
                } else {
                    //node, check if it contains the point we want
                    if(!currentBoundingBox.containsPoint(coordinate)) {
                        indexesToProcess.pop(); //box doesn't contain the point, can skip all points and nodes below
                    } else {
                        //box contains the point, must subdivide, pop this index, and then add the 8 child nodes to be processed
                        indexesToProcess.pop();
                        currentChildAABBs = getChildAABBs(currentBoundingBox);
                        for(int i = Octree::CAPACITY-1; i >= 0; --i) {
                            indexesToProcess.push(NodeData{(static_cast<std::int32_t>(nodes.at(currentIndex) & (~static_cast<std::uint32_t>(0b11u << 30u))) + i), currentChildAABBs.at(i)});
                        }
                    }
                }
            }

            return false;
        }

        [[nodiscard]] std::vector<Coordinate> pointsInAABB(const BoundingBox box) {
            std::vector<Coordinate> pointsInRange;

            if(!rootNode.intersectsAABB(box)) {
                return pointsInRange; //empty list since no points fall outside of rootNode
            }

            struct NodeData {
                std::int32_t index;
                BoundingBox aabb;
            };

            std::array<BoundingBox, CAPACITY> currentChildAABBs = getChildAABBs(rootNode);

            std::stack<NodeData> indexesToProcess;
            for(std::int32_t i = Octree::CAPACITY-1; i >= 0; --i) {
                indexesToProcess.push(NodeData{i, currentChildAABBs.at(i)});
            }

            std::int32_t currentIndex; //tracks first element of the 8 nodes
            BoundingBox currentBoundingBox;
            while(!indexesToProcess.empty()) {
                currentIndex = indexesToProcess.top().index;
                currentBoundingBox = indexesToProcess.top().aabb;

                if(nodes.at(currentIndex) >> 31u) {
                    //empty leaf, do nothing
                    indexesToProcess.pop();
                } else if(!((nodes.at(currentIndex) >> 30u) & 1u)) {
                    //filled leaf, check if the point falls into the range
                    const auto data = leaves.at((nodes.at(currentIndex) & (~static_cast<std::uint32_t>(0b11u << 30u))) - 1u);
                    if(box.containsPoint(data)) {
                        pointsInRange.push_back(data);
                    }
                    indexesToProcess.pop();
                } else {
                    //node, check if it intersects the range
                    if(!box.intersectsAABB(currentBoundingBox)) {
                        indexesToProcess.pop(); //box doesn't overlap, can skip all points and nodes below
                    } else {
                        //box overlaps, must subdivide, pop this index, and then add the 8 child nodes to be processed
                        indexesToProcess.pop();
                        currentChildAABBs = getChildAABBs(currentBoundingBox);
                        for(int i = Octree::CAPACITY-1; i >= 0; --i) {
                            indexesToProcess.push(NodeData{(static_cast<std::int32_t>(nodes.at(currentIndex) & (~static_cast<std::uint32_t>(0b11u << 30u))) + i), currentChildAABBs.at(i)});
                        }
                    }
                }
            }

            return pointsInRange;
        }

        [[nodiscard]] std::optional<Coordinate> firstRayintersection(const Ray ray) const {
            const std::vector<Coordinate> intersectedPoints = rayIntersections(ray);

            if(intersectedPoints.empty()) {
                std::cout << "ray missed terrain\n";
                return std::nullopt;
            }

            glm::vec3 min = intersectedPoints[0];
            for(const auto c : intersectedPoints) {
                if(distSquared(ray.origin, c) < distSquared(ray.origin, min)) {
                    min = c;
                }
            }

            return min;
        }

        [[nodiscard]] std::vector<Coordinate> rayIntersections(const Ray ray) const {
            std::vector<Coordinate> pointsIntersected;

            if(!rootNode.intersectsRay(ray)) {
                std::cout << "ray missed terrain root node\n";
                return pointsIntersected; //empty list since ray doesn't intersect rootNode
            }

            struct NodeData {
                std::int32_t index;
                BoundingBox aabb;
            };

            std::array<BoundingBox, CAPACITY> currentChildAABBs = getChildAABBs(rootNode);

            std::stack<NodeData> indexesToProcess;
            for(std::int32_t i = Octree::CAPACITY-1; i >= 0; --i) {
                indexesToProcess.push(NodeData{i, currentChildAABBs.at(i)});
            }

            std::int32_t currentIndex; //tracks first element of the 8 nodes
            BoundingBox currentBoundingBox;
            while(!indexesToProcess.empty()) {
                currentIndex = indexesToProcess.top().index;
                currentBoundingBox = indexesToProcess.top().aabb;

                if(nodes.at(currentIndex) >> 31u) {
                    //empty leaf, do nothing
                    indexesToProcess.pop();
                } else if(!((nodes.at(currentIndex) >> 30u) & 1u)) {
                    //filled leaf, add it to the list since the box has been hit by the ray
                    pointsIntersected.push_back(leaves.at((nodes.at(currentIndex) & (~static_cast<std::uint32_t>(0b11u << 30u))) - 1u));
                    indexesToProcess.pop();
                } else {
                    //node, check if it intersects the ray
                    if(!currentBoundingBox.intersectsRay(ray)) {
                        indexesToProcess.pop(); //box doesn't intersect the ray, can skip all points and nodes below
                    } else {
                        //box intersects the ray, must subdivide, pop this index, and then add the 8 child nodes to be processed
                        indexesToProcess.pop();
                        currentChildAABBs = getChildAABBs(currentBoundingBox);
                        for(int i = Octree::CAPACITY-1; i >= 0; --i) {
                            indexesToProcess.push(NodeData{(static_cast<std::int32_t>(nodes.at(currentIndex) & (~static_cast<std::uint32_t>(0b11u << 30u))) + i), currentChildAABBs.at(i)});
                        }
                    }
                }
            }

            return pointsIntersected;
        }

        friend std::ostream& operator<<(std::ostream &stream, const Octree &octree) {
            stream << '\n';

            stream << "Tree Size: " << octree.nodes.size() << '\n';

            struct NodeInfo {
                std::int32_t index;
                std::int32_t treeLevel;
            };

            std::stack<NodeInfo> indexesToProcess;
            for(int i = Octree::CAPACITY-1; i >= 0; --i) {
                indexesToProcess.push(NodeInfo{i, 0});
            }

            std::int32_t currentIndex; //tracks first element of the 8 nodes
            std::int32_t currentTreeLevel;
            while (!indexesToProcess.empty()) {
                currentIndex = indexesToProcess.top().index;
                currentTreeLevel = indexesToProcess.top().treeLevel;

                if(octree.nodes.at(currentIndex) >> 31u) {
                    //empty leaf
                    stream << "index: " << currentIndex << ", treeLevel: " << currentTreeLevel << ", data: std::nullopt\n";
                    indexesToProcess.pop();
                } else if(!((octree.nodes.at(currentIndex) >> 30u) & 1u)) {
                    //filled leaf
                    stream << "index: " << currentIndex << ", treeLevel: " << currentTreeLevel << ", data: " << octree.leaves.at((octree.nodes.at(currentIndex) & (~static_cast<std::uint32_t>(0b11u << 30u))) - 1u) << '\n';
                    indexesToProcess.pop();
                } else {
                    //parent node
                    stream << "index: " << currentIndex << ", treeLevel: " << currentTreeLevel << ", nextIndex: " << (octree.nodes.at(currentIndex) & (~static_cast<std::uint32_t>(0b11u << 30u))) << '\n';
                    indexesToProcess.pop();
                    for(int i = Octree::CAPACITY-1; i >= 0; --i) {
                        indexesToProcess.push(NodeInfo{(static_cast<std::int32_t>(octree.nodes.at(currentIndex) & (~static_cast<std::uint32_t>(0b11u << 30u))) + i), currentTreeLevel+1});
                    }
                }
            }

            stream << '\n';

            return stream;
        }
    };
}
