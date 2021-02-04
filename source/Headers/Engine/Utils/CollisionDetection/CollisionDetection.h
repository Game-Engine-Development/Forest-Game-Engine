#pragma once

#include <cmath>
#include <algorithm>

#include <glm/glm.hpp>

#include "Headers/Engine/Utils/MiscUtils.h"
#include "Headers/Engine/Utils/CommonDeclarations.h"
#include "Headers/Engine/Scene/Components.h"

/*
struct GeometryVertex {
    glm::vec3 pos, normal;
    std::vector<std::uint32_t> edges;
};

struct Edge {
    std::array<std::uint32_t, 2> vertices;
    std::vector<std::uint32_t> faces;
};

//for now, all triangles in mesh are faces
struct Face {
    glm::vec3 normal;
    std::vector<std::uint32_t> edges;
};

struct BoundaryRepMesh {
    std::vector<GeometryVertex> vertices;
    std::vector<Edge> edges;
    std::vector<Face> faces;
};


inline BoundaryRepMesh convertToBoundaryRep(const Component::Mesh &mesh) {
    BoundaryRepMesh retMesh{};

    //anything marked with `{}` is a temporary placeholder value
    for(unsigned int i = 0u; i < mesh.indices.size(); i += 3u) {
        //currently duplicates vertices, faces, and triangles, will deduplicate later
        GeometryVertex vertex0{.pos=mesh.vertices.at(mesh.indices.at(i)).position, .normal={}, .edges={i, i+2}};
        GeometryVertex vertex1{.pos=mesh.vertices.at(mesh.indices.at(i+1)).position, .normal={}, .edges={i, i+1}};
        GeometryVertex vertex2{.pos=mesh.vertices.at(mesh.indices.at(i+2)).position, .normal={}, .edges={i+1, i+2}};

        Edge edge0{.vertices={i, i+1}, .faces={i%3u}};
        Edge edge1{.vertices={i+1, i+2}, .faces={i%3u}};
        Edge edge2{.vertices={i+2, i}, .faces={i%3u}};

        Face face{.normal={}, .edges={i, i+1, i+2}};

        retMesh.vertices.push_back(vertex0);
        retMesh.vertices.push_back(vertex1);
        retMesh.vertices.push_back(vertex2);

        retMesh.edges.push_back(edge0);
        retMesh.edges.push_back(edge1);
        retMesh.edges.push_back(edge2);

        retMesh.faces.push_back(face);
    }

    return retMesh;
}
*/

/*
struct GeometryVertex {
    glm::vec3 position, normal;
};

struct ConvexShape {
    std::vector<GeometryVertex> vertices;
};

struct Face {
    glm::vec3 vertex, normal;
};

struct Edge {
    glm::vec3 edge, vertex;
};


struct ConvexPolyhedron {
    std::vector<Face> faces;
    std::vector<Edge> edges;
    std::vector<GeometryVertex> vertices;
};
*/

/*
using Point = glm::vec3;
inline int whichSide(const std::vector<GeometryVertex> &S, const Point D, const Point P) {
    //S vertices are projected to the form P+t*D. Return value is +1 if all t>0,
    // -1 if all t<0, 0 otherwise, in which case the line splits the polygon.

    //@todo use bools
    int positive = 0, negative = 0;
    for(int i = 0; i < S.size(); ++i) {
        const float t = glm::dot(D, S.at(i).pos-P);

        if(t > 0) {
            ++positive;
        } else if(t < 0) {
            ++negative;
        } else if(positive && negative) {
            return 0;
        }
    }

    return (positive ? +1 : -1);
}

inline bool testIntersection3D(const BoundaryRepMesh &C0, const BoundaryRepMesh &C1) {
    //Test faces of C0 for separation. Because of the counterclockwise ordering,
    // the projection interval for C0 is [m,0] where m<=0. Only try to determine
    // if C1 is on the "positive" side of the line.

    for(int i = 0; i < C0.faces.size(); ++i) {
        const glm::vec3 D = C0.faces.at(i).normal; //outward pointing
        if(whichSide(C1.vertices, D, C0.vertices.at(C0.edges.at(C0.faces.at(i).edges.at(0)).vertices.at(0)).pos) > 0) {
            //C1 is entirely on "positive" side of line C0.F(i).vertex+t*D
            return false;
        }
    }



    //Test faces of C1 for separation. Because of the counterclockwise ordering,
    // the projection interval for C1 is [m,0] where m<=0. Only try to determine
    // if C0 is on the "positive" side of the line.

    for(int i = 0; i < C1.faces.size(); ++i) {
        const glm::vec3 D = C1.faces.at(i).normal; //outward pointing
        if(whichSide(C0.vertices, D, C1.vertices.at(C1.edges.at(C1.faces.at(i).edges.at(0)).vertices.at(0)).pos) > 0) {
            //C1 is entirely on "positive" side of line C0.F(i).vertex+t*D
            return false;
        }
    }



    //Test cross product of pairs of edges, one from each polygon.
    for(int i = 0; i < C0.edges.size(); ++i) {
        for(int j = 0; j < C1.edges.size(); ++j) {
            const glm::vec3 D = glm::cross(C0.vertices.at(C0.edges.at(i).vertices.at(1)).pos, C1.vertices.at(C1.edges.at(i).vertices.at(1)).pos);

            const int side0 = whichSide(C0.vertices, D, C0.vertices.at(C0.edges.at(i).vertices.at(0)).pos);
            if(side0 == 0) {
                continue;
            }

            const int side1 = whichSide(C1.vertices, D, C0.vertices.at(C0.edges.at(i).vertices.at(0)).pos);
            if(side1 == 0) {
                continue;
            }

            if(side0*side1 < 0) {
                //C0 and C1 are on "opposite" sides of line C0.E(i).vertex+t*D
                return false;
            }
        }
    }

    return true;
}

inline glm::vec2 perp(const glm::vec2 line) {
    return glm::vec2{line.y, -line.x};
}
*/





struct BoundingBox {
    Coordinate center{};
    LengthVec3 halfWidths{};

    [[nodiscard]] bool containsPoint(Coordinate point) const noexcept;

    [[nodiscard]] bool intersectsAABB(const BoundingBox &other) const noexcept;

    [[nodiscard]] bool intersectsRay(Ray r) const noexcept;

    [[nodiscard]] glm::vec3 get_vmin() const noexcept;
    [[nodiscard]] glm::vec3 get_vmax() const noexcept;
};



struct BoundingSphere {
    Coordinate center{};
    double radius{};

    [[nodiscard]] constexpr bool isPointInsideSphere(Coordinate point) const noexcept;

    [[nodiscard]] bool intersectsOtherSphere(BoundingSphere other) const noexcept;

    [[nodiscard]] bool intersectWithBox(BoundingBox box) const noexcept;
};









/*
use std::collections::HashMap;

struct Vector3([f32; 3]);

struct Mesh {
    vertices: Vec<Vector3>,
    // inefficient storage, but this is purely for demonstration purposes
    faces: Vec<Vec<usize>>
}

struct Vertex {
    position: Vector3,
    edges: Vec<Edge>,
}

struct Edge {
    // indices into the vertices list
    vertices: [usize; 2],
    // indices into the faces list
    faces: Vec<usize>,
}

struct Face {
    // indices into the edges list
    edges: Vec<usize>,
}

struct BoundaryRepMesh {
    vertices: Vec<Vertex>,
    edges: Vec<Edge>,
    faces: Vec<Face>,
}

// First element is always less than second element
#[derive(Copy, Clone, Eq, PartialEq, Hash, Ord)]
struct UnorderedPair<T>(T, T);

impl<T: Ord> From<(T, T)> for UnorderedPair {
    fn from((a, b): (T, T)) -> UnorderedPair {
        if a < b {
            UnorderedPair(a, b)
        } else {
            UnorderedPair(b, a)
        }
    }
}

impl From<Mesh> for BoundaryRepMesh {
    fn from(mesh: Mesh) -> BoundaryRepMesh {
        let vertices: Vec<_> = mesh.vertices.iter()
            .map(|v| Vertex { position: v, edges: Vec::new() }).collect();
        let edges = Vec::new();
        let edge_map = HashMap::new();
        let faces: Vec<_> = mesh.faces.iter().enumerate()
            .map(|(face_index, f)| {
                let face = Face { edges: Vec::new() };
                for (i, v0) in f.iter().enumerate() {
                    let v1 = f[(i + 1) % f.len()];
                    let pair = UnorderedPair::from((v0, v1));
                    let edge_index = edge_map.entry(&pair).get_or_insert_with(|| {
                        let index = edges.len();
                        edges.push(Edge {
                            vertices: [v0, v1],
                            faces: Vec::new()
                        });
                        index
                    });
                    edges[edge_index].faces.push(face_index);
                    face.edges.push(edge_index);
                }
            })
            .collect();
        BoundaryRepMesh {
            vertices,
            edges,
            faces,
        }
    }
}
*/
