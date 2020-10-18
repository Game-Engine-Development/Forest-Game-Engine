#version 410

layout(location = 0) in vec3 vertex_position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// use z position to shader darker to help perception of distance
out float dist;

void main() {
    gl_Position = projection * view * model * vec4 (vertex_position, 1.0);
    dist = vertex_position.z; //1.0 - (-pos_eye.z / 10.0);
}
