#version 300 es

precision mediump float;

in vec4 aColor;
in vec2 aTexCoord;

out vec4 fragColor;

uniform sampler2D uTexture0;

void main() {
    fragColor = aColor * texture(uTexture0, aTexCoord);
}