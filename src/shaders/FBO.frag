#version 420 core

in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D text;

void main() {
	FragColor = texture(text, texCoords);
}