#version 330 core

layout (location = 0) in vec3 inFragPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTextureCoord;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBiTangent;

out vec2 fTextureCoord;
out vec3 fNormal;
out mat3 fTBN;
out vec3 fFragPosition;

uniform mat4 projectionAndView;
uniform mat4 model;

void main(){
	vec4 vertPosition = model * vec4(inFragPosition, 1.0);
	gl_Position = projectionAndView * vertPosition;

	fTextureCoord = inTextureCoord;
	fNormal = mat3(model) * inNormal;

	fFragPosition = vertPosition.xyz;

	vec3 T = normalize((model*vec4(inTangent, 0.0f)).xyz);
	vec3 B = normalize((model*vec4(inBiTangent, 0.0f)).xyz);
	vec3 N = normalize((model*vec4(inNormal, 0.0f)).xyz);

	fTBN = mat3(T, B, N);
}
