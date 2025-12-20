#version 330 core //specify OpenGl version used
out vec4 FragColor; //The shader outputs a vec4 color

//taking a vec3 "color" variable as input. 
//It is passed down by the vertex shader directly. 
//So its very important that the ouput var of the vertex shader and this input var have the exact same name
in vec3 color; 

void main()
{
   FragColor = vec4(color, 1.0f);
}

//NB:
//this file is in GLSL language, not C/C++