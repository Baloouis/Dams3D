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
uniform sampler2D tex1;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;


vec4 pointLight()
{
	vec3 lightVec = currentPos - lightPos;
	float dist = length(lightVec);
	float a = 0.2f; //called quadratic term
	float b = 0.075f; //called linear term
	float intensityDistCoef = 1.0f / (a * dist * dist + b * dist + 1.0f);


	//ambient lightning
	float ambient = 0.20f; //to prevent pitch black darkness when object are not lit directly ( thus emulating natural ambiant light reflection in real life )

	//diffuse lightning (main lightning cast over the object by the light)
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec); //dir from light pos to current world pos
	//NB : we put the opposite direction of lightDirection for the dot product because we want a brighter value when the normal and the vector from curWorldPos to lightPos get closer ( not vector from lightPos to curWorldPos)
	//we put max() with 0.0 to get rid of negative values. 
	float diffuse = max(dot(normal, -lightDirection), 0.0f); 

	//computing specular lightning ( ie direct reflection from the light source on the object, going towards the camera)
	float specularLight = 0.5f; //max intensity of specular lighting
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(lightDirection, normal);
	float specAmount = max(dot(viewDirection, reflectionDirection), 0.f);
	specAmount = pow(specAmount, 16);
	float specular = specAmount * specularLight;

	//return final color
	return lightColor * (texture(tex0, texCoord) * (diffuse * intensityDistCoef + ambient) + specular * texture(tex1, texCoord).r * intensityDistCoef);
}

vec4 directionalLight()
{
	vec3 lightDirection = normalize(vec3(-1.0f,-1.0f,0.0f)); // hard setted for directional light as all light rays are considered to be parallel

	//ambient lightning
	float ambient = 0.20f; //to prevent pitch black darkness when object are not lit directly ( thus emulating natural ambiant light reflection in real life )
	//diffuse lightning (main lightning cast over the object by the light)
	vec3 normal = normalize(Normal);
	
	//NB : we put the opposite direction of lightDirection for the dot product because we want a brighter value when the normal and the vector from curWorldPos to lightPos get closer ( not vector from lightPos to curWorldPos)
	//we put max() with 0.0 to get rid of negative values. 
	float diffuse = max(dot(normal, -lightDirection), 0.0f); 

	//computing specular lightning ( ie direct reflection from the light source on the object, going towards the camera)
	float specularLight = 0.5f; //max intensity of specular lighting
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(lightDirection, normal);
	float specAmount = max(dot(viewDirection, reflectionDirection), 0.f);
	specAmount = pow(specAmount, 16);
	float specular = specAmount * specularLight;

	//return final color
	return lightColor * (texture(tex0, texCoord) * (diffuse + ambient) + specular * texture(tex1, texCoord).r);
}


vec4 spotLight()
{
	//From center of light (angle = 0) to inner cone angle we have light intensity = 1. Then there's a fade out between inner cone angle and outer cone angle. 
	//We stated here the cos of the angle correspondings to the inner/outer cone rather than the angle themselves for efficiency purposes
	float innerCone = 0.95f; //corresponds to an angle of around 26f degres
	float outerCone = 0.9f; //corresponds to an angle of around 18f degres

	vec3 lightVec = currentPos - lightPos;

	//ambient lightning
	float ambient = 0.20f; //to prevent pitch black darkness when object are not lit directly ( thus emulating natural ambiant light reflection in real life )

	//diffuse lightning (main lightning cast over the object by the light)
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec); //dir from light pos to current world pos
	//NB : we put the opposite direction of lightDirection for the dot product because we want a brighter value when the normal and the vector from curWorldPos to lightPos get closer ( not vector from lightPos to curWorldPos)
	//we put max() with 0.0 to get rid of negative values. 
	float diffuse = max(dot(normal, -lightDirection), 0.0f); 

	//computing specular lightning ( ie direct reflection from the light source on the object, going towards the camera)
	float specularLight = 0.5f; //max intensity of specular lighting
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(lightDirection, normal);
	float specAmount = max(dot(viewDirection, reflectionDirection), 0.f);
	specAmount = pow(specAmount, 16);
	float specular = specAmount * specularLight;

	//computing angle intensity for spotlight based on the angle between currentWorldPos and the center of the light cone 
	//So if lightDirection vector get close to down vector (0,-1,0) it means we're getting underneath the spotLight;
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), lightDirection);
	float intensityAngleCoef = clamp((angle - outerCone)/(innerCone - outerCone), 0.0f, 1.0f);

	//return final color
	return lightColor * (texture(tex0, texCoord) * (diffuse * intensityAngleCoef + ambient) + specular * texture(tex1, texCoord).r * intensityAngleCoef);
}

void main()
{
	//outputs final color
	FragColor = pointLight();
}
