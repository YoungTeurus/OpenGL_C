#version 330 core

out vec4 outColor;

in vec4 fColor;
in vec2 fTexCoord;

uniform sampler2D uTexture;
uniform int uDrawMode;  // Тип рисования - цветом или текстурой

void main()
{
	switch (uDrawMode){
		case 0:
			outColor = fColor;
			break;
		case 1:
			outColor = texture(uTexture, fTexCoord);
			break;
	}
}