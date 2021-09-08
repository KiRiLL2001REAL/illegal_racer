#version 300 es

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoord;

out vec4 aColor;
out vec2 aTexCoord;

uniform mat4 model;
uniform mat4 projview;

void main() {
   aColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
   aTexCoord = vTexCoord;
   gl_Position = projview * model * vec4(vPosition, 1.0f);
}