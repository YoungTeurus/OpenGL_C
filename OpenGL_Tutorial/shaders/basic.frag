#version 330 core

out vec4 outColor;

in vec4 fColor;
in vec2 fTextureCoord;

uniform sampler2D uTexture;
uniform sampler2D uTexture2;
uniform int uDrawMode;  // Тип рисования - цветом, текстурой или двумя текстурами

void main()
{
	switch (uDrawMode){
		case 0:
			outColor = fColor;
			break;
		case 1:
			outColor = texture(uTexture, fTextureCoord);
			break;
		case 2:
			outColor = mix(texture(uTexture, fTextureCoord), texture(uTexture2, fTextureCoord), 0.5);
			break;
	}
}