#version 430 core

in vec2 TexCoord;
in vec3 fragNormal;
in vec3 fragVert;

out vec4 FragColor;

uniform vec3 camPos;
uniform vec3 lightPos;
uniform mat4 model;
uniform sampler2D ourTexture;
uniform vec3 lightColor;
uniform vec4 vertexColor;
uniform float specularity;

void main() {
	// Invisible if alhpa is low
	vec4 tex = texture(ourTexture, TexCoord) * vertexColor;
	if(tex.a < 0.1)
		discard;

	// Ambient
	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * lightColor;


	// Diffuse
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normalMatrix * fragNormal);
	
	vec3 fragPosition = vec3(model * vec4(fragVert, 1.0));
	vec3 surfaceToLight = normalize(lightPos - fragPosition);

	float brightness = max(dot(normal, surfaceToLight), 0.1);
	vec3 diffuse = brightness * lightColor;


	// specular
	vec3 viewDir = normalize(camPos - fragVert);
	vec3 reflectDir = reflect(-surfaceToLight, normal);
	float spec = 0.0;
	vec3 halfwayDir = normalize(surfaceToLight + viewDir);  
	spec = pow(max(dot(normal, halfwayDir), 0.0), specularity);
	vec3 specular = spec * lightColor;    
	
	float dist = distance(lightPos, fragVert);
	float attenuation = 1.0f / (1.0 + (0.03125 * dist) + (0.0 * dist * dist));

	// final colors
	vec4 surfaceColor = texture(ourTexture, TexCoord) * vertexColor;
	FragColor = vec4((ambient + diffuse * attenuation) * surfaceColor.rgb + specular * attenuation, 1);

	// Black and White
	FragColor = vec4((FragColor.r+FragColor.g+FragColor.b)/3.0f, (FragColor.r+FragColor.g+FragColor.b)/3.0f, (FragColor.r+FragColor.g+FragColor.b)/3.0f, FragColor.a);
}