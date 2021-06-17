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

vec3 getNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal, float magnitude){
	vec3 direction = normal * magnitude;
	return position + vec4(direction, 0.0);
}

void main(){
	vec3 normal = getNormal();
	// vec3 normal = gs_in[0].fNormal;
	
	gl_Position = explode(gl_in[0].gl_Position, normal, uExplosionMagnitude);
	gTextureCoord = vTextureCoord[0];
	gNormal = vNormal[0];
	gTBN = vTBN[0];
	gFragPosition = vFragPosition[0];
	EmitVertex();
	
	gl_Position = explode(gl_in[1].gl_Position, normal, uExplosionMagnitude);
	gTextureCoord = vTextureCoord[1];
	gNormal = vNormal[1];
	gTBN = vTBN[1];
	gFragPosition = vFragPosition[1];
	EmitVertex();
	
	gl_Position = explode(gl_in[2].gl_Position, normal, uExplosionMagnitude);
	gTextureCoord = vTextureCoord[2];
	gNormal = vNormal[2];
	gTBN = vTBN[2];
	gFragPosition = vFragPosition[2];
	EmitVertex();

	// gl_Position = gl_in[0].gl_Position;
	// gTextureCoord = vTextureCoord[0];
	// gNormal = vNormal[0];
	// gTBN = vTBN[0];
	// gFragPosition = vFragPosition[0];
	// EmitVertex();
	// 
	// gl_Position = gl_in[1].gl_Position;
	// gTextureCoord = vTextureCoord[1];
	// gNormal = vNormal[1];
	// gTBN = vTBN[1];
	// gFragPosition = vFragPosition[1];
	// EmitVertex();
	// 
	// gl_Position = gl_in[2].gl_Position;
	// gTextureCoord = vTextureCoord[2];
	// gNormal = vNormal[2];
	// gTBN = vTBN[2];
	// gFragPosition = vFragPosition[2];
	// EmitVertex();

	EndPrimitive();
}