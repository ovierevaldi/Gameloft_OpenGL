#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_textcoord;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_textcoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
    vs_position = vec4(ModelMatrix * vec4(vertex_position, 1.f)).xyz;
    vs_color = vertex_color;
    vs_textcoord = vec2(vertex_textcoord.x , vertex_textcoord.y * -1.f);

    gl_Position = ModelMatrix * vec4(vertex_position, 1.f);
}