#version 330 core

layout (location = 0) in vec3 aPos; 

//All transformation matrices ( needed for 3D viewing with perspective )
uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
   gl_Position = camMatrix * model * vec4(aPos, 1.0);
}
