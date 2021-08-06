#version 300 es

layout (location = 0) in vec3 vPosition;

out vec4 aColor;

void main() {
   aColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
   gl_Position = vec4(vPosition, 1);
}