#pragma once
#include <iostream>
#include <glad/glad.h>

class DrawFrameBuffer
{
public:
	unsigned bufferId;
	unsigned drawTextureId;
	
	DrawFrameBuffer(int textureWidth, int textureHeight)
	{
		// ��������� � ��������:
		// �������� ������������:
		glGenFramebuffers(1, &bufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, bufferId);

		// �������� ��������, �� �������� ����� ��������� ������ frameBuffer-�.
		glGenTextures(1, &drawTextureId);
		glBindTexture(GL_TEXTURE_2D, drawTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		// ������������ ��������� �������� � framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, drawTextureId, 0);

		// �������� render-������ ��� depth � stencil ������:
		// Render-����� �������� � ������ write-only.
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, textureWidth, textureHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// ������������ ��������� buffer � framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}

		// ������������� �� ����������� framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};
