#version 330 core //specify OpenGl version used
out vec4 FragColor; //The shader outputs a vec4 color

//taking a vec3 "color" variable as input. 
//It is passed down by the vertex shader directly. 
//So its very important that the ouput var of the vertex shader and this input var have the exact same name
in vec3 color; 
in vec2 texCoord; 
in vec3 Normal; //NB : not necessarily normalized yet !
in vec3 currentPos; // current world pos of our vertice


uniform sampler2D tex0; //sampler1D/2D/3D is just the OpenGL Built-in data type for textures. We use it to pass access to the texture we want to the fragment shader

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
	//ambient lightning
	float ambient = 0.20f; //to prevent pitch black darkness when object are not lit directly ( thus emulating natural ambiant light reflection in real life )

	//diffuse lightning (main lightning cast over the object by the light)
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - currentPos); //dir from current world pos towards light pos
	float diffuse = max(dot(normal, lightDirection), 0.0f); //we put max() with 0.0 to get rid of negative values

	//computing specular lightning ( ie direct reflection from the light source on the object, going towards the camera)
	float specularLight = 0.50f; //max intensity of specular lighting
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal); //negative lightDirection to get reflection of lightning going towards the plane, not away from it
	float specAmount = max(dot(viewDirection, reflectionDirection), 0.f);
	specAmount = pow(specAmount, 8);
	float specular = specAmount * specularLight;

	//outputs final color
	FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specular);
}
