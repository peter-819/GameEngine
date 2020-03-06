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
	float brightness = 0.6*dot(theNormal,lightVector);
	vec4 diffuseLight = vec4(brightness,brightness,brightness,1);

	//specular Light
	vec3 reflectedLightVector = reflect(-lightVector,theNormal);
	vec3 CameraVector = normalize(CameraPosition - thePosition);
	float specularity = clamp(dot(reflectedLightVector,CameraVector),0,1);
	
	float r = 8.0, d = length(LightPosition - thePosition);
	float kc = 1, kl = 2.0/r, kq = 1.0/(r*r);
	float attenuation = 1.0 / (kc + kl * d + kq * d * d);
	
	specularity = attenuation* pow(specularity,20);

	vec4 specularLight = vec4(0.0, 0.0, 0.0, 0.0);
	if(dot(lightVector,theNormal) > 0)
		specularLight = vec4(specularity,specularity,specularity,1);
	
	//float finalBrightness = max(0.1,brightness+specularity);
	//daColor = vec4(finalBrightness,finalBrightness,finalBrightness,1);
	daColor = AmbientLight + clamp(diffuseLight,0,1) + clamp(specularLight,0,1);
	//daColor = clamp(specularLight,0,1);
}