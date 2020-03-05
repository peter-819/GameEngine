#version 430

out vec4 daColor;

in vec3 thePosition;
in vec3 theNormal;

uniform vec3 LightPosition;
uniform vec4 AmbientLight;
uniform vec3 CameraPosition;

void main()
{
	//diffuse light
	vec3 lightVector = normalize(LightPosition - thePosition);
	float brightness = 0.8*dot(theNormal,lightVector);
	vec4 diffuseLight = vec4(brightness,brightness,brightness,1);

	//specular Light
	vec3 reflectedLightVector = reflect(-lightVector,theNormal);
	vec3 CameraVector = normalize(CameraPosition - thePosition);
	float specularity = dot(reflectedLightVector,CameraVector);
	specularity = 0.25 * pow(specularity,20);
	vec4 specularLight = vec4(specularity,specularity,specularity,1);
	daColor =  AmbientLight + clamp(diffuseLight,0,1) + clamp(specularLight,0,1);
}