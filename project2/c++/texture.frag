#version 150

// Texture mapping vertex shader
//
// Contributor:  Thomas Landi

// INCOMING DATA

// Here is where you should add the variables you need in order
// to propogate data from the vertex shader to the fragment shader
// so that it can do the shading and texture mapping computations

//Texture
uniform sampler2D myTexture;

//Texture Coordinates
in vec2 texCoord;

// OUTGOING DATA

out vec4 finalColor;

void main()
{
    // Replace this code with your implementation of this shader
	//Set the color to the texture at the provided coordinates
	vec4 texColor = texture(myTexture,texCoord);
	
    finalColor = texColor;
}
