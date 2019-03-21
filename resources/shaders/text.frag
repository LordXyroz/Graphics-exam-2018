#version 430 core

in vec4 vertexColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec4 ourColor;
uniform sampler2D ourTexture;

void main() {
	//FragColor = vertexColor;
	FragColor = texture(ourTexture, TexCoord);
	if (FragColor.a < 0.1f)
		discard;
	else
		FragColor.a = 1.0f;
}