#version 330 core //specify OpenGl version used

//layout info helps OpenGL read the Data it receives
//for exemple first line below says that on the 0th layout, there is vec3 datatype for positions

layout (location = 0) in vec3 aPos; //The shader is taking an input vec3 called "aPos"
layout (location = 1) in vec3 aColor;

out vec3 color; //outputting a color for the vertex, to be passed down and used by the fragment shader later on

//declare a float uniform variable
//a uniform variable are kind of global variables that can be access within other shaders but also our main program file directly:
//In main program file, get the value with glGetUniformLocation(...) and then glGetUniformuiv(...)
//in other shaders just declare the same line as below and then use the "scale" variable, just like here
uniform float scale; 

void main()
{
   //Using openGL keyword "gl_Position" used for the vertex position.
   //Here its kinda like our shader method was outputting the gl_Position value ( even though its not exactly the case)
   gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);

   color = aColor;
}

//NB:
//this file is in GLSL language, not C/C++