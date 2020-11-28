#pragma once

#include <iostream>
#include <cstddef>
#include <array>
#include <vector>
#include <cassert>
#include <memory>
#include <cstdio>
#include <cinttypes>
#include <type_traits>
#include <optional>
#include <initializer_list>
#include <memory_resource>
#include <forward_list>

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

    struct NTreeNode {
        bool isLeaf; //used for tagging the union
        union {
            //if not a leaf, stores index to start of next node
            // (of which there are `CAPACITY` children on that level next to each other)
            std::uint32_t childNodeIndex;

            //if leaf, store the data
            Coordinate data;
        };
    };

    //for nodes: std::nullopt means the node is empty, false and non-null means it contains data (is a leaf),
    //           and true and non-null means it is a parent node
    std::vector<std::optional<NTreeNode>> nodes;

    static constexpr std::uint32_t startTreeLevel = 0;
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
                std::cerr << "skipping point, out of range of root node\n";
                toProcess.pop();
            } else {
                for (int i = 0; i < currentChildAABBs.size(); ++i) {
                    if (currentChildAABBs.at(i).containsPoint(currentData)) {
                        if (!nodes.at(currentIndex + i).has_value()) {
                            //empty leaf, can just insert currentData
                            nodes.at(currentIndex + i) = NTreeNode{.isLeaf=true, .data=currentData};

                            toProcess.pop(); //remove the data from the toProcess list

                            //reset the index and aabb to root for the next data
                            currentChildAABBs = getChildAABBs(rootNode);
                            currentIndex = 0;
                        } else if (nodes.at(currentIndex + i)->isLeaf) {
                            const auto dataInTree = nodes.at(currentIndex + i)->data;
                            if((mathRound(dataInTree.x, 5) == mathRound(currentData.x, 5)) && (mathRound(dataInTree.y, 5) == mathRound(currentData.y, 5)) && (mathRound(dataInTree.z, 5) == mathRound(currentData.z, 5))) {
                                //if the same value already exists in the tree
                                std::cerr << "skipping point, duplicate\n";
                                toProcess.pop();

                                //reset the index and aabb to root for the next data
                                currentChildAABBs = getChildAABBs(rootNode);
                                currentIndex = 0;
                            } else {
                                //filled leaf, will need to insert this leaf, resize, and reinsert currentData
                                toProcess.push(nodes.at(currentIndex + i)->data);

                                //elements are default constructed to std::nullopt
                                nodes.resize(nodes.size() + CAPACITY);
                                nodes.at(currentIndex + i) = NTreeNode{.isLeaf=false,
                                        .childNodeIndex=static_cast<std::uint32_t>((nodes.size() - CAPACITY))};

                                //move the index and AABBs to new children
                                currentChildAABBs = getChildAABBs(currentChildAABBs.at(i));
                                currentIndex = nodes.size() - CAPACITY;
                            }
                        } else {
                            //node, just continue going down by updating the index to the start of the next child nodes
                            currentChildAABBs = getChildAABBs(currentChildAABBs.at(i));
                            currentIndex = nodes.at(currentIndex + i)->childNodeIndex;
                        }

                        break; //AABB has been found, stop iterating through them
                    }
                }
            }
        }
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

            if(!octree.nodes.at(currentIndex).has_value()) {
                //empty leaf
                stream << "index: " << currentIndex << ", treeLevel: " << currentTreeLevel << ", data: std::nullopt\n";
                indexesToProcess.pop();
            } else if(octree.nodes.at(currentIndex)->isLeaf) {
                //filled leaf
                stream << "index: " << currentIndex << ", treeLevel: " << currentTreeLevel << ", data: " << octree.nodes.at(currentIndex)->data << '\n';
                indexesToProcess.pop();
            } else {
                //parent node
                stream << "index: " << currentIndex << ", treeLevel: " << currentTreeLevel << ", nextIndex: " << octree.nodes.at(currentIndex)->childNodeIndex << '\n';
                indexesToProcess.pop();
                for(int i = Octree::CAPACITY-1; i >= 0; --i) {
                    indexesToProcess.push(NodeInfo{static_cast<std::int32_t>(octree.nodes.at(currentIndex)->childNodeIndex + i), currentTreeLevel+1});
                }
            }
        }

        stream << '\n';

        return stream;
    }
};
}