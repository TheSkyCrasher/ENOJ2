#pragma once

#include <GL\glew.h>
#include "window.h"

class FrameBuffer
{
public:
	FrameBuffer(unsigned int width = Window::GetWidth(), unsigned int height = Window::GetHeight());
	virtual ~FrameBuffer();

	void BindForWriting();
	void BindForReading(unsigned int unit);
private:
	unsigned int m_width;
	unsigned int m_height;

	GLuint CreateColorTexture(unsigned int width, unsigned int height);

	GLuint m_frameBuffer;
	GLuint m_renderTexture;
};