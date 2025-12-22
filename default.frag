#version 330 core //specify OpenGl version used
out vec4 FragColor; //The shader outputs a vec4 color

//taking a vec3 "color" variable as input. 
//It is passed down by the vertex shader directly. 
//So its very important that the ouput var of the vertex shader and this input var have the exact same name
in vec3 color; 
in vec2 texCoord; 

uniform sampler2D tex0; //sampler1D/2D/3D is just the OpenGL Built-in data type for textures. We use it to pass access to the texture we want to the fragment shader

void main()
{
   //FragColor = vec4(color, 1.0f);
   FragColor = texture(tex0, texCoord); //texture() is just openGL buit-in method to sample color value from a texture
}

//NB:
//this file is in GLSL language, not C/C++