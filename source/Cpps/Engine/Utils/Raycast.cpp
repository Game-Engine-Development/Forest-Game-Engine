#include "Headers/Engine/Utils/Raycast.h"

/*glm::vec3 getRayFromScreenSpace(const glm::vec2 pos, const Camera &camera, const Window &window) {
    const glm::mat4 invMat = inverse(camera.getProjectionMatrix()*camera.getViewMatrix());

    const float halfScreenWidth = window.getWidth()/2.0f;
    const float halfScreenHeight = window.getHeight()/2.0f;

    const glm::vec4 near = glm::vec4((pos.x - halfScreenWidth) / halfScreenWidth, -1*(pos.y - halfScreenHeight) / halfScreenHeight, -1, 1.0);
    const glm::vec4 far = glm::vec4((pos.x - halfScreenWidth) / halfScreenWidth, -1*(pos.y - halfScreenHeight) / halfScreenHeight, 1, 1.0);

    glm::vec4 nearResult = invMat*near;
    glm::vec4 farResult = invMat*far;
    nearResult /= nearResult.w;
    farResult /= farResult.w;

    const glm::vec3 dir = glm::vec3(farResult - nearResult );

    return normalize(dir);
}*/

[[nodiscard]] std::optional<float> get_ray_plane_intersection_t(const glm::vec3 origin, const glm::vec3 ray, const float planeHeight) noexcept{
    //the ray is parallel to plane
    if (mathRound(ray.y) == 0.f) {
        if (mathRound(origin.y) != mathRound(planeHeight)) return std::nullopt;

        //else, the plane and ray have infintely many points of intersection, feel free to return `std::nullopt` instead
        return 0.f;
    }

    //the ray diverges from plane (points away from plane)
    if(origin.y > planeHeight && ray.y > 0.f) return std::nullopt;
    if(planeHeight > origin.y && 0.f > ray.y) return std::nullopt;

    return (planeHeight - origin.y)/ray.y;
}

glm::vec3 get_ray_from_mouse(const Window &window, const Camera &camera, const float mouse_x, const float mouse_y) {
    // screen space (viewport coordinates)
    const float x = (2.0f * mouse_x) / window.getWidth() - 1.0f;
    const float y = 1.0f - (2.0f * mouse_y) / window.getHeight();
    // normalised device space
    const glm::vec3 ray_nds = glm::vec3(x, y, 1.0f);
    // clip space
    const glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
    // eye space
    glm::vec4 ray_eye = glm::inverse(camera.getProjectionMatrix()) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
    // world space
    glm::vec3 ray_wor = glm::vec3(glm::inverse(camera.getViewMatrix()) * ray_eye);
    // don't forget to normalise the vector at some point
    ray_wor = glm::normalize(ray_wor);
    return ray_wor;
}

bool ray_sphere(const glm::vec3 ray_origin_wor, const glm::vec3 ray_direction_wor, const glm::vec3 sphere_centre_wor, const float sphere_radius, float &intersection_distance) {
    // work out components of quadratic
    const glm::vec3 dist_to_sphere = ray_origin_wor - sphere_centre_wor;
    const float b = glm::dot(ray_direction_wor, dist_to_sphere);
    const float c = glm::dot(dist_to_sphere, dist_to_sphere) - (sphere_radius * sphere_radius);
    const float b_squared_minus_c = (b * b) - c;
    // check for "imaginary" answer. == ray completely misses sphere
    if (b_squared_minus_c < 0.0f) return false;
    // check for ray hitting twice (in and out of the sphere)
    if (b_squared_minus_c > 0.0f) {
        // get the 2 intersection distances along ray
        const float t_a = -b + std::sqrt(b_squared_minus_c);
        const float t_b = -b - std::sqrt(b_squared_minus_c);
        intersection_distance = t_b;
        // if behind viewer, throw one or both away
        if (t_a < 0.0f) {
            if (t_b < 0.0f) return false;
        } else if (t_b < 0.0) {
            intersection_distance = t_a;
        }

        return true;
    }
    // check for ray hitting once (skimming the surface)
    if (0.0f == b_squared_minus_c) {
        // if behind viewer, throw away
        const float t = -b + std::sqrt(b_squared_minus_c);
        if (t < 0.0f) return false;
        intersection_distance = t;
        return true;
    }
    // note: could also check if ray origin is inside sphere radius
    return false;
}
