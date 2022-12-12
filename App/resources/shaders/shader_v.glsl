const std::string vertexShaderSrc = R"(
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture;

uniform mat4 u_model;
uniform mat3 u_normal;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 rotation;
uniform bool line = false;

flat out vec4 vs_color;
out vec4 vs_color_interp;
out vec2 vs_tcoords;
out vec3 vs_position;
out vec3 vs_fragPos;
out vec3 vs_normal;


void main() {
    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);
    vs_color = line ? vec4(0.0f, 0.0f, 0.0f, 0.0f) : vec4(normal, 1.0f);
    vs_normal = normalize(u_normal * normal);
    vs_fragPos = vec3(u_model * vec4(position, 1.0));
    vs_tcoords = texture;
    vs_position = position;
}
)";
