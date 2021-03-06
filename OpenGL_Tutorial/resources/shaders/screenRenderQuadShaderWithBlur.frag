#version 330 core
in vec2 fTexCoords;

out vec4 fragColor;

uniform sampler2D image;

uniform bool horizontal;
uniform float weight[6] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216,
                                    0.013);

void main(){
	vec2 texture_offset = 1.0 / textureSize(image, 0);
	vec3 result = texture(image, fTexCoords).rgb * weight[0];

	if(horizontal)
    {
        for(int i = 1; i < 6; ++i)
        {
            result += texture(image, fTexCoords + vec2(texture_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(image, fTexCoords - vec2(texture_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 6; ++i)
        {
            result += texture(image, fTexCoords + vec2(0.0, texture_offset.y * i)).rgb * weight[i];
            result += texture(image, fTexCoords - vec2(0.0, texture_offset.y * i)).rgb * weight[i];
        }
    }

	fragColor = vec4(result, 1.0);
}