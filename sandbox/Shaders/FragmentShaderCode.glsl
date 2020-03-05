#version 430

out vec4 daColor;

in vec3 thePosition;
in vec3 theNormal;

uniform vec3 LightPosition;

void main()
{
	vec3 lightVector = normalize(LightPosition - thePosition);
	float brightness = dot(theNormal,lightVector);
	daColor = vec4(brightness,brightness,brightness,1);
}