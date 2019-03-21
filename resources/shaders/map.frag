#version 430 core

in vec3 fragNormal;
in vec3 fragVert;
in vec4 viewspace;

out vec4 FragColor;

uniform vec3 camPos;
uniform vec3 lightPos;
uniform mat4 model;
uniform sampler2D ourTexture;
uniform vec3 lightColor;
uniform float specularity;

uniform float waterHeight;
uniform float beachHeight;
uniform float vegitationHeight;
uniform float mountainHeight;

uniform vec4 waterColor;
uniform vec4 beachColor;
uniform vec4 vegitationColor;
uniform vec4 mountainColor;
uniform vec4 snowColor;

uniform int hardlines;
uniform float maxHeight;
uniform int enableSpec;
uniform int enableFog;
uniform int noNormals;

void main() {
	// Ambient
	float ambientStrength = 0.15 + noNormals;
	vec3 ambient = ambientStrength * lightColor;


	// Diffuse
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normalMatrix * fragNormal);
	
	vec3 fragPosition = vec3(model * vec4(fragVert, 1.0));
	vec3 surfaceToLight = normalize(lightPos - fragPosition);

	float brightness = min(dot(normal, surfaceToLight), 1.0);
	
	float t = (lightPos.y / 1000);
	brightness = (brightness / 20) * (1.0 - t) + brightness * t;
	if (lightPos.y < 0) brightness = 0;

	vec3 diffuse = brightness * lightColor * 2 * (1.0 - noNormals); 
	
	vec3 specular = vec3(0, 0, 0);

	
	// specular
	vec3 specColor = vec3(1.0, 1.0, 1.0);
	vec3 viewDir = normalize(camPos - fragVert);
	vec3 reflectDir = reflect(-surfaceToLight, normal);
	float spec = 0.0;
	vec3 halfwayDir = normalize(surfaceToLight + viewDir);  
	spec = pow(max(dot(normal, halfwayDir), 0.0), specularity);
	if (surfaceToLight.y < 0) spec = 0;
	specular = spec * specColor;

	vec4 surfaceColor;

	if (fragVert.y < waterHeight) {
		surfaceColor = waterColor;
	}

	else if (fragVert.y >= waterHeight && fragVert.y < beachHeight) {
		surfaceColor = beachColor;
	}

	else if (fragVert.y >= beachHeight && fragVert.y < vegitationHeight) {
		float t = 1.9;
		if (hardlines == 0)
			t = (fragVert.y - beachHeight) / (vegitationHeight - beachHeight) + 0.9;
		t = t / 1.9;
		surfaceColor = (1.0 - t) * beachColor + t * vegitationColor;
	}

	else if (fragVert.y >= vegitationHeight && fragVert.y < mountainHeight) {
		float t = 1;
		if (hardlines == 0)
			t = (fragVert.y - vegitationHeight) / (mountainHeight - vegitationHeight);
		surfaceColor = (1.0 - t) * vegitationColor + t * mountainColor;
	}

	else {
		float t = 1;
		if (hardlines == 0)
			t = (fragVert.y - mountainHeight) / (maxHeight - mountainHeight);
		surfaceColor = (1.0 - t) * mountainColor + t * snowColor;
	}

	vec4 fogColor = vec4(0.7, 0.7, 0.7, 1.0);

	float dist = length(viewspace);
	float fogFactor = (1000 - dist)/(1000 - 500);

	if (fogFactor <= 0.0) fogColor = vec4(0.7, 0.7, 0.7, 0.0);

   	fogFactor = clamp(fogFactor, 0.0, 1.0);
	if (enableFog == 0) fogColor = vec4(surfaceColor.rgb, 0.0);

	// final colors
	if (enableSpec == 1 || fragVert.y < waterHeight)
		surfaceColor = vec4((ambient + diffuse) * surfaceColor.rgb, 1);
	else
		surfaceColor = vec4((ambient + diffuse) * surfaceColor.rgb, 1);

	FragColor = mix(fogColor, surfaceColor, fogFactor);
	if (enableFog == 0) FragColor = vec4(surfaceColor.rgb, 1.0);
}