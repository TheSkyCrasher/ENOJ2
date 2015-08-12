#include "skybox.h"
#include "stb_image.h"
#include <iostream>

CubemapTexture::CubemapTexture(const std::string& filename)
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	GLuint types[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	};

	for (unsigned int i = 0; i < 6; i++) {
		int width, height, numComponents;
		unsigned char* data = stbi_load(("Data/images/skybox/" + filename + std::to_string(i) + ".jpg").c_str(), &width, &height, &numComponents, 4);

		if (data == NULL)
			std::cerr << "Unable to load texture: " << "Data/images/skybox/" + filename + std::to_string(i) + ".jpg" << std::endl;

		glTexImage2D(types[i], 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}

CubemapTexture::~CubemapTexture()
{
	glDeleteTextures(1, &m_texture);
}

void CubemapTexture::Bind(unsigned int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}

SkyBox::SkyBox() : m_shader("skybox")
{
	m_cube = new Model("cube.obj");
	m_texture = new CubemapTexture("skybox");
}

SkyBox::~SkyBox()
{
	if (m_cube) delete m_cube;
	if (m_texture) delete m_texture;
}

void SkyBox::Draw(Camera* camera)
{
	m_shader.Bind();

	GLint oldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
	GLint oldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);

	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	m_shader.SetUniform("MVP", camera->GetViewProjection() * Matrix4f().InitTranslation(camera->GetPos()));
	m_cube->DrawMesh(0);

	glCullFace(oldCullFaceMode);
	glDepthFunc(oldDepthFuncMode);
}