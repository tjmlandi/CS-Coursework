#version 150

// Diffuse fragment shader
//
// Contributor:  Thomas Landi

//Colors for the diffuse color, direction, light ambience, and minimum brightness
uniform vec3 lightColor;
uniform vec3 lightDirection;
uniform float ambience;
uniform float minBrightness;

// INCOMING DATA

//Normal and position vectors
in vec3 normal;

// OUTGOING DATA
out vec4 finalColor;

void main()
{
	vec4 tmp = vec4(1.0,1.0,1.0,1.0);
	float diffuseIntensity = max(minBrightness, dot(normalize(normal), -normalize(lightDirection)));
	finalColor = vec4(lightColor, 1.0) * vec4(lightColor * (ambience * diffuseIntensity) * tmp.rgb, 1.0);
}
