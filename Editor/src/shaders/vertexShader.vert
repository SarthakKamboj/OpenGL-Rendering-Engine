#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 color;
out vec2 texCoords;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	color = aColor;
	color = (aNormal * 0.5) + 0.5;
	texCoords = aTexCoords;
}