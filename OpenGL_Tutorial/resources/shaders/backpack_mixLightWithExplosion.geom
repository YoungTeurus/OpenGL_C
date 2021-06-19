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

uniform float uExplosionMagnitude = 1.5;
uniform float gravity = 9.8;
uniform vec3 gravityDirection = vec3(0.0, -1.0, 0.0);
uniform float uTimeSinceExplosion = 0.0;

vec3 getNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal, float magnitude){
	vec3 direction = normal * magnitude;
	vec3 gravityFall = vec3(0.0, -1.0 * uTimeSinceExplosion, 0.0);
	return position + vec4(direction, 0.0) + vec4( gravityFall , 1.0);
}

void main(){
	vec3 normal = getNormal();
	vec4 newPosition;
	// vec3 normal = gs_in[0].fNormal;
	
	newPosition = explode(gl_in[0].gl_Position, normal, uExplosionMagnitude);
	gl_Position = newPosition;
	gTextureCoord = vTextureCoord[0];
	gNormal = vNormal[0];
	gTBN = vTBN[0];
	gFragPosition = vFragPosition[0];
	// gFragPosition = newPosition.xyz;
	EmitVertex();
	
	newPosition = explode(gl_in[1].gl_Position, normal, uExplosionMagnitude);
	gl_Position = newPosition;
	gTextureCoord = vTextureCoord[1];
	gNormal = vNormal[1];
	gTBN = vTBN[1];
	gFragPosition = vFragPosition[1];
	// gFragPosition = newPosition.xyz;
	EmitVertex();
	
	newPosition = explode(gl_in[2].gl_Position, normal, uExplosionMagnitude);
	gl_Position = newPosition;
	gTextureCoord = vTextureCoord[2];
	gNormal = vNormal[2];
	gTBN = vTBN[2];
	gFragPosition = vFragPosition[2];
	// gFragPosition = newPosition.xyz;
	EmitVertex();

	EndPrimitive();
}