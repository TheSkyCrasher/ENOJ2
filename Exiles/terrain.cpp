#include "terrain.h"
#include "stb_image.h"

TerrainChunk::TerrainChunk(std::vector<Vertex> vertices, std::vector<int> indices)
{
	m_indicesNum = indices.size();
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesNum * sizeof(int), &indices[0], GL_STATIC_DRAW);
}

TerrainChunk::~TerrainChunk()
{
	if (m_vbo) glDeleteBuffers(1, &m_vbo);
	if (m_vbo) glDeleteBuffers(1, &m_ibo);
}

Terrain::Terrain(const std::string& fileName) : m_shader("terrain")
{
	m_heightScale = 100.0f;

	int width, height, numComponents;
	unsigned char* data = stbi_load(("Data/images/" + fileName).c_str(), &width, &height, &numComponents, 1);
	if (data == NULL)
		std::cerr << "Unable to load texture: " << "Data/images/" + fileName << std::endl;

	m_width = width;
	m_height = height;

	m_heigtmap = new unsigned char*[height];
	for (int i = 0; i < height; ++i)
	{
		m_heigtmap[i] = new unsigned char[width];
		for (int j = 0; j < width; ++j)
		{
			m_heigtmap[i][j] = data[i*height + j];
		}
	}

	std::vector<Vertex> vertices;
	std::vector<int> indices;

	float UVTiling = m_width * 100.0f; // if m_height == m_width
	for (int i = 0; i < m_height; ++i)
	{
		for (int j = 0; j < m_width; ++j)
		{
			float scaleX = (float)j / (float)m_width;
			float scaleY = (float)i / (float)m_height;
			float vertexHeight = GetHeight((float)i,(float)j);

			Vertex vert((-0.5f + scaleX)*2000.0f, vertexHeight  - m_heightScale/2.0f, (-0.5f + scaleY)*2000.0f, UVTiling * m_width * scaleX, UVTiling * m_height * scaleY, 0, 0, 0);
			vertices.push_back(vert);
		}
	}
	std::cout << vertices.size() << "\n";
	for (int i = 0; i < m_height-1; ++i)
	{
		for (int j = 0; j < m_width-1; ++j)
		{
			indices.push_back(i * m_width + j);             //  0---*
			indices.push_back((i + 1) * m_width + j);       //  | \ |
			indices.push_back(i * m_width + j + 1);         //  0---0

			indices.push_back((i + 1) * m_width + j);       //  0---0
			indices.push_back((i + 1) * m_width + j + 1);   //  | \ |
			indices.push_back(i * m_width + j + 1);         //  *---0
		}
	}
	std::cout << indices.size() << "\n";
	m_indicesNum = indices.size();
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesNum * sizeof(int), &indices[0], GL_STATIC_DRAW);
	stbi_image_free(data);
}

Terrain::~Terrain()
{
	for (int i = 0; i < m_height; ++i) {
		delete[] m_heigtmap[i];
	}
	delete[] m_heigtmap;
	if (m_vbo) glDeleteBuffers(1, &m_vbo);
	if (m_vbo) glDeleteBuffers(1, &m_ibo);
}

void Terrain::Draw(const Matrix4f& mvp)
{
	m_shader.Bind();
	m_shader.SetUniform("MVP", mvp);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vector3f));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3f)+sizeof(Vector2f)));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3f)* 2 + sizeof(Vector2f)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glDrawElements(GL_TRIANGLES, m_indicesNum, GL_UNSIGNED_INT, 0);
}