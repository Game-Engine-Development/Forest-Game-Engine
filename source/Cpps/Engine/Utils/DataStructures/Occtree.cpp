#include "Headers/Engine/Utils/DataStructures/Octtree.h"

BoundingBox::BoundingBox(const Coordinate point, const std::array<float, 3> halfWidths)
        : center(point), halfWidths(halfWidths)
{}


[[nodiscard]] bool BoundingBox::containsPoint(const Coordinate point) const noexcept {
    const bool containsX = point.x < (center.x + halfWidths[0]) && point.x >= (center.x - halfWidths[0]);
    const bool containsY = point.y < (center.y + halfWidths[1]) && point.y >= (center.y - halfWidths[1]);
    const bool containsZ = point.z < (center.z + halfWidths[2]) && point.z >= (center.z - halfWidths[2]);

    return containsX && containsY && containsZ;
}

[[nodiscard]] bool BoundingBox::intersectsAABB(const BoundingBox &other) const noexcept {
    const bool edgeX = ((other.center.x - other.halfWidths[0]) <= (center.x + halfWidths[0]) && (other.center.x - other.halfWidths[0]) >= (center.x - halfWidths[0]))
                       || ((other.center.x + other.halfWidths[0]) >= (center.x - halfWidths[0]) && (other.center.x + other.halfWidths[0]) <= (center.x + halfWidths[0]));

    const bool edgeY = ((other.center.y - other.halfWidths[1]) <= (center.y + halfWidths[1]) && (other.center.y - other.halfWidths[1]) >= (center.y - halfWidths[1]))
                       || ((other.center.y + other.halfWidths[1]) >= (center.y - halfWidths[1]) && (other.center.y + other.halfWidths[1]) <= (center.y + halfWidths[1]));

    const bool edgeZ = ((other.center.z - other.halfWidths[2]) <= (center.z + halfWidths[2]) && (other.center.z - other.halfWidths[2]) >= (center.z - halfWidths[2]))
                       || ((other.center.z + other.halfWidths[2]) >= (center.z - halfWidths[2]) && (other.center.z + other.halfWidths[2]) <= (center.z + halfWidths[2]));


    const bool smallerX = ((other.center.x + other.halfWidths[0]) >= (center.x + halfWidths[0]) && (other.center.x - other.halfWidths[0]) <= (center.x - halfWidths[0]));
    const bool smallerY = ((other.center.y + other.halfWidths[1]) >= (center.y + halfWidths[1]) && (other.center.y - other.halfWidths[1]) <= (center.y - halfWidths[1]));
    const bool smallerZ = ((other.center.z + other.halfWidths[2]) >= (center.z + halfWidths[2]) && (other.center.z - other.halfWidths[2]) <= (center.z - halfWidths[2]));


    return ((edgeX || smallerX) && (edgeY || smallerY) && (edgeZ || smallerZ));
}




Octtree::Octtree(const BoundingBox &boundary)
        : Octtree(8, boundary, 0)
{}

Octtree::Octtree(const std::size_t capacity, const BoundingBox &boundary, const std::size_t treeLevel)
        : treeLevel(treeLevel), capacity(capacity), boundary(boundary)
{}


bool Octtree::insert(const Coordinate p) {
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

void Octtree::subdivide() {
    const auto getNewBoundingBox = [&](const int i) {
        const std::array newHalfWidths{boundary.halfWidths[0]/2.0f, boundary.halfWidths[1]/2.0f, boundary.halfWidths[2]/2.0f};

        const auto getPartition = [&] (const std::array<bool, 3> negative) {
            std::array<float, 3> halfWidths{};
            for(int index = 0; index < 3; ++index) {
                halfWidths[index] = (negative[index]) ? -newHalfWidths[index] : newHalfWidths[index];
            }
            return BoundingBox{Coordinate{boundary.center.x + halfWidths[0], boundary.center.y + halfWidths[1], boundary.center.z + halfWidths[2]}, newHalfWidths};
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

    //std::cout << "subdivide\n";
    for(int i = 0; i < children.size(); ++i) {
        children[i] = std::make_unique<Octtree>(capacity, getNewBoundingBox(i), treeLevel+1);
    }
}

[[nodiscard]] std::vector<Coordinate> Octtree::queryRange(const BoundingBox range) const {
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


std::ostream& operator<< (std::ostream &stream, const Octtree &octtree) {
    static std::size_t lastTreeLevel = 0;
    if(octtree.treeLevel != lastTreeLevel) {
        std::cout << '\n';
        lastTreeLevel = octtree.treeLevel;
    }
    stream << "size: " << octtree.points.size() << ", treeLevel: " << octtree.treeLevel << '\n';
    for(const auto &child : octtree.children) {
        if(child) stream << *child;
    }
    return stream;
}
