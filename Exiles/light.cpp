#include "light.h"
#include <iostream>

DirectionalLight::DirectionalLight() : m_shader("light")
{
	m_depthTexture = CreateDepthTexture(DEPTH_TEXTURE_WIDTH, DEPTH_TEXTURE_HEIGHT);
	m_frameBuffer = 0;
	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTexture, 0);

	GLenum fboStatus;
	if ((fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("glCheckFramebufferStatus error 0x%X\n", fboStatus);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_direction = Vector3f(0.0f, -0.5f, 0.4f);
	Matrix4f projection = Matrix4f().InitPerspective(ToRadians(60.0f), 1.3f, 1.0f, 200.0f);
	Matrix4f rotation = Matrix4f().InitRotationFromDirection(m_direction, Vector3f(0.0f, 1.0f, 0.0f));
	Matrix4f position = Matrix4f().InitTranslation(Vector3f(0.0f, 5.5f, -7.69f) * -1.0f);
	m_MVP = projection * rotation * position;

	m_shader.AddUniform<Matrix4f>("lightMV");
	m_shader.AddUniform<Matrix4f>("MP");
}

DirectionalLight::~DirectionalLight()
{
	glDeleteTextures(1, &m_depthTexture);
	glDeleteFramebuffers(1, &m_frameBuffer);
}

void DirectionalLight::SetRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glViewport(0, 0, DEPTH_TEXTURE_WIDTH, DEPTH_TEXTURE_HEIGHT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_TRUE);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);

	m_shader.Bind();

	m_shader.Update("lightMV", m_MVP);
}

void DirectionalLight::BindTexture(unsigned int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
}

GLuint DirectionalLight::CreateDepthTexture(unsigned int width, unsigned int height)
{
	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	return texture;
}