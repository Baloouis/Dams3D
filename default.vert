#version 330 core //specify OpenGl version used
//Positions/Coordinates
layout (location = 0) in vec3 aPos; //The shader is taking an input vec3 called "aPos"
//Normals ( NB : not necessarily normalized ! )
layout (location = 1) in vec3 aNormal;
//Color
layout (location = 2) in vec3 aColor;
//Texture Coordinates
layout (location = 3) in vec2 aTex;

out vec3 Normal;
out vec3 color; //outputting a color for the vertex, to be passed down and used by the fragment shader later on
out vec2 texCoord;
out vec3 currentPos;

//Uniform variables are kind of qglobal variables that can be access both within other shaders but also our main program file directly
//All transformation matrices ( needed for 3D viewing with perspective )
uniform mat4 model;
uniform mat4 camMatrix;
//Light infos
uniform vec3 lightPos;

void main()
{
    currentPos = vec3(model * vec4(aPos, 1.0f));

   //Using openGL keyword "gl_Position" used for the vertex position.
   //Here its kinda like our shader method was outputting the gl_Position value ( even though its not exactly the case)

   gl_Position = camMatrix * vec4(currentPos, 1.0);

   //Passing vertex data to the fragment shader
   Normal = aNormal;
   color = aColor;
   texCoord = aTex;
}