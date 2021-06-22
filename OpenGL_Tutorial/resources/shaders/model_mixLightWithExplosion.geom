#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 vTextureCoord[];
in vec3 vNormal[];
in mat3 vTBN[];
in vec3 vFragPosition[];

out vec2 gTextureCoord;
out vec3 gNormal;
out mat3 gTBN;
out vec3 gFragPosition;

uniform mat4 projectionAndView;

uniform float uExplosionMagnitude = 0.0;

vec3 getNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal){
	vec3 direction = normal * uExplosionMagnitude;
	return position + vec4(direction, 0.0);
}

void main(){
	vec3 normal = getNormal();

	vec4 newPosition = projectionAndView * explode(gl_in[0].gl_Position, normal);
	gl_Position = newPosition;
	gTextureCoord = vTextureCoord[0];
	gNormal = vNormal[0];
	gTBN = vTBN[0];
	gFragPosition = vFragPosition[0];
	EmitVertex();

	newPosition = projectionAndView * explode(gl_in[1].gl_Position, normal);
	gl_Position = newPosition;
	gTextureCoord = vTextureCoord[1];
	gNormal = vNormal[1];
	gTBN = vTBN[1];
	gFragPosition = vFragPosition[1];
	EmitVertex();

	newPosition = projectionAndView * explode(gl_in[2].gl_Position, normal);
	gl_Position = newPosition;
	gTextureCoord = vTextureCoord[2];
	gNormal = vNormal[2];
	gTBN = vTBN[2];
	gFragPosition = vFragPosition[2];
	EmitVertex();

	EndPrimitive();
}
