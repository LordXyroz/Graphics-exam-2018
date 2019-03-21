#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normals;

out vec4 vertexColor;
out vec2 TexCoord;
out vec3 fragNormal;
out vec3 fragVert;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	fragVert = position;
	fragNormal = normals;
	vertexColor = vec4(1.0, 1.0, 1.0, 1.0);

	TexCoord = texCoord;
	gl_Position = projection * view * model * vec4(position, 1.0);
}