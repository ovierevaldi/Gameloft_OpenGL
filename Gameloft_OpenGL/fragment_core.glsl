#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_textcoord;

out vec4 fs_color;

uniform sampler2D texture0;

void main()
{
	fs_color = texture(texture0, vs_textcoord) * vec4(vs_color, 0.01f);
}