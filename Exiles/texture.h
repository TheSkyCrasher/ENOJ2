#pragma once

#include <string>
#include <GL/glew.h>
#include <map>
#include "util.h"

class TextureData : public ReferenceCounter
{
public:
	TextureData(GLenum textureTarget);
	virtual ~TextureData();

	inline GLenum GetTextureTarget() { return m_textureTarget; }
	inline GLuint GetTextureID() { return m_textureID; }
protected:
private:
	TextureData(TextureData& other) {}
	void operator=(TextureData& other) {}

	GLenum m_textureTarget;
	GLuint m_textureID;
};

class Texture
{
public:
	Texture(const std::string& fileName, bool transparent = false);

	void Bind(unsigned int unit);

	virtual ~Texture();
protected:
private:
	static std::map<std::string, TextureData*> s_resourceMap;

	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}

	TextureData* m_textureData;
	std::string m_fileName;

	void InitTexture(int width, int height, unsigned char* data, GLenum textureTarget, GLfloat filter);
};