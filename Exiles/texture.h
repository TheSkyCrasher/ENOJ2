#pragma once

#include <string>
#include <GL/glew.h>

class Texture
{
public:
	Texture(const std::string& fileName);
	Texture(const GLuint& texture) : m_texture(texture) {}

	void Bind(unsigned int unit);

	GLuint GetUint() { return m_texture; }

	virtual ~Texture();
protected:
private:
	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}

	GLuint m_texture;
};