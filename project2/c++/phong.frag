#version 150

// Phong fragment shader
//
// Contributor:  Thomas Landi

//Colors for the specular and diffuse lighting, as well as the position of the light
uniform vec3 diffuseColor;
uniform vec3 lightPosition;
uniform vec3 specularColor;

// INCOMING DATA

// Here is where you should add the variables you need in order
// to propogate data from the vertex shader to the fragment shader
// so that it can do the shading computations
//Normal and position vectors
in vec3 normal;
in vec3 position;

// OUTGOING DATA
out vec4 finalColor;

void main()
{
    // Replace this code with your implementation of this shader
	//Calculate the normal and direction of the light
	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(lightPosition - position);
	//Get the lambertian
	float lambertian = max(dot(lightDirection,norm), 0);
	
	float specular = 0.0;
	
	//If we have a non zero light
	if (lambertian > 0.0) {
		//Calculate reflection and view directions
		vec3 reflectDirection = reflect(-lightDirection, norm);
		vec3 viewDirection = normalize(-position);

		//Calculate the specular intensity and angle
		float specularAngle = max(dot(reflectDirection, viewDirection), 0.0);
		specular = pow(specularAngle, pow(4.0,16.0));
	}

	//Calculate the final color from the diffuse and specular calculations
	finalColor = vec4(lambertian*diffuseColor + specular*specularColor, 1.0);
}
