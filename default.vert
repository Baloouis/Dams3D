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
uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

//Light infos
uniform vec3 lightPos;

void main()
{
    //the "-" for the rotation is maybe needed because glm and the gltm model format used different rotation standards ( one clockwise and the other counter clockwise )
    currentPos = vec3(model * translation * rotation * scale * vec4(aPos, 1.0f)); 

   //Using openGL keyword "gl_Position" used for the vertex position.
   //Here its kinda like our shader method was outputting the gl_Position value ( even though its not exactly the case)

   gl_Position = camMatrix * vec4(currentPos, 1.0);

   //Passing vertex data to the fragment shader
   Normal = aNormal;
   color = aColor;
   texCoord = mat2(1.0, 0.0, 0.0, -1.0) * aTex; //The mat2() and *  operation are used to rotate the texture 90 degrees. Might be needed because of different standards for reading/writing textures
}