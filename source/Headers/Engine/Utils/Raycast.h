#pragma once

#include <cmath>
#include <optional>

#include <glm/glm.hpp>

#include "Headers/Engine/IO/Input.h"
#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/IO/Window.h"
#include "Headers/Engine/Utils/MiscUtils.h"

//glm::vec3 getRayFromScreenSpace(glm::vec2 pos, const Camera &camera, const Window &window);

//this is the t to plugin in to get the point of intersection on the ray (ray*t = point of intersection)
[[nodiscard]] std::optional<float> get_ray_plane_intersection_t(glm::vec3 origin, glm::vec3 ray, float planeHeight) noexcept;

/* takes mouse position on screen and return ray in world coords */
glm::vec3 get_ray_from_mouse(const Window &window, const Camera &camera, float mouse_x, float mouse_y);

/* check if a ray and a sphere intersect. if not hit, returns false. it rejects
intersections behind the ray caster's origin, and sets intersection_distance to
the closest intersection */
bool ray_sphere(glm::vec3 ray_origin_wor, glm::vec3 ray_direction_wor, glm::vec3 sphere_centre_wor, float sphere_radius, float &intersection_distance);
