#include "Primitive.h"
#include <hmath.h>

Primitive::Primitive()
{
}


Primitive::~Primitive()
{
}

using namespace hm;

Quad::Quad(vec3 center, float sizeX, float sizeY) : Quad(center, sizeX, sizeY, 1, 0){}

Quad::Quad(vec3 center, float sizeX, float sizeY, int textureNumRows, int textureIndex)
{
	model = new IndexedModel();

	this->index = (float)textureIndex;
	this->numRows = (float)textureNumRows;
	this->m_center = center;

	calcAtlas();

	vec3 topL = vec3(center.x - sizeX, center.y + sizeY, 0);	// 0
	vec3 topR = vec3(center.x + sizeX, center.y + sizeY, 0);	// 1
	vec3 botL = vec3(center.x - sizeX, center.y - sizeY, 0);	// 2
	vec3 botR = vec3(center.x + sizeX, center.y - sizeY, 0);	// 3

	model->getPositions()->push_back(topL);
	model->getPositions()->push_back(topR);
	model->getPositions()->push_back(botL);
	model->getPositions()->push_back(botR);

	vec3 xaxis(1,0,0);
	mat4 rotation_mat = mat4::rotate(xaxis, -ANGLE);
	vec4 normal_v4 = rotation_mat * vec4(0, 0, 1, 1);
	vec3 normal = vec3(normal_v4.x, normal_v4.y, normal_v4.z);
	model->getNormals()->push_back(normal);
	model->getNormals()->push_back(normal);
	model->getNormals()->push_back(normal);
	model->getNormals()->push_back(normal);

	model->getTexCoords()->push_back(vec2(0, 1));
	model->getTexCoords()->push_back(vec2(1, 1));
	model->getTexCoords()->push_back(vec2(0, 0));
	model->getTexCoords()->push_back(vec2(1, 0));

	model->getIndices()->push_back(0);
	model->getIndices()->push_back(3);
	model->getIndices()->push_back(1);
	model->getIndices()->push_back(0);
	model->getIndices()->push_back(2);
	model->getIndices()->push_back(3);
}

Quad::Quad(vec3& topLeftCorner, vec3& bottomRightCorner)
{
	model = new IndexedModel();

	this->index = 0.0f;
	this->numRows = 1.0f;

	vec3 bottomLeft = vec3(topLeftCorner.x, bottomRightCorner.y, topLeftCorner.z);
	vec3 topRight = vec3(bottomRightCorner.x, topLeftCorner.y, bottomRightCorner.z);

	model->positions.push_back(topLeftCorner);
	model->positions.push_back(topRight);
	model->positions.push_back(bottomLeft);
	model->positions.push_back(bottomRightCorner);

	vec3 normal = vec3(0, 1, 0);
	model->normals.push_back(normal);
	model->normals.push_back(normal);
	model->normals.push_back(normal);
	model->normals.push_back(normal);

	model->texCoords.push_back(vec2(0, 0));	// 0
	model->texCoords.push_back(vec2(1, 0));	// 1
	model->texCoords.push_back(vec2(0, 1));	// 2
	model->texCoords.push_back(vec2(1, 1));	// 3

	model->indices.push_back(0);
	model->indices.push_back(3);
	model->indices.push_back(1);
	model->indices.push_back(0);
	model->indices.push_back(2);
	model->indices.push_back(3);
}

void Quad::updateQuad(int width, int height)
{
	vec3 topL = vec3(m_center.x - width, m_center.y + height, 0);	// 0
	vec3 topR = vec3(m_center.x + width, m_center.y + height, 0);	// 1
	vec3 botL = vec3(m_center.x - width, m_center.y - height, 0);	// 2
	vec3 botR = vec3(m_center.x + width, m_center.y - height, 0);	// 3

	model->positions.clear();

	model->getPositions()->push_back(topL);
	model->getPositions()->push_back(topR);
	model->getPositions()->push_back(botL);
	model->getPositions()->push_back(botR);
}

void Quad::calcAtlas()
{
	if (numRows == 0)
		return;
	int column = (int)index % (int)numRows;
	offset.x = (float)column / (float)numRows;
	int row = (int)index / (int)numRows;
	offset.y = (float)row / (float)numRows;
}

Quad::~Quad()
{
	if (model)
		delete model;
}

float Quad::getTextureIndex()
{
	return index;
}

float Quad::getTextureNumRows()
{
	return numRows;
}

vec2& Quad::getTextureOffset()
{
	return offset;
}

void Quad::setIndex(int i)
{
	this->index = (float)i;
	calcAtlas();
}

float Quad::getIndex()
{
	return this->index;
}

IndexedModel* Quad::getIndexedModel()
{
	return model;
}

void Border::genVAO()
{
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
}

void Border::genVBOS()
{
	glBindVertexArray(VertexArrayID);
	glGenBuffers(1, &VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, model->positions.size() * sizeof(float) * 3, &model->positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDisableVertexAttribArray(0);

	// Normals Data
	glGenBuffers(1, &NormalsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, NormalsBufferID);
	glBufferData(GL_ARRAY_BUFFER, model->normals.size() * sizeof(float) * 3, &model->normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDisableVertexAttribArray(1);

	// Texture Data
	glGenBuffers(1, &TextureBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, TextureBufferID);
	glBufferData(GL_ARRAY_BUFFER, model->texCoords.size() * sizeof(float) * 2, &model->texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glDisableVertexAttribArray(2);
}

void Border::genIndexBuffer()
{
	glBindVertexArray(VertexArrayID);
	glGenBuffers(1, &IndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->indices.size() * sizeof(unsigned int), &model->indices[0], GL_STATIC_DRAW);
}

Border::Border(hm::vec3 center, float width, float height, float left_size, float right_size, float top_size, float bottom_size)
{
	model = new IndexedModel();

	bg_color = hm::vec4(1, 1, 1, 1);

	float half_width = width / 2.0f;
	float half_height = height / 2.0f;

	// topL topR botL botR
	model->positions.push_back(vec3(center.x - half_width - left_size, center.y + half_height + top_size, 0.0f));
	model->positions.push_back(vec3(center.x - half_width, center.y + half_height, 0.0f));
	model->positions.push_back(vec3(center.x - half_width - left_size, center.y - half_height - bottom_size, 0.0f));
	model->positions.push_back(vec3(center.x - half_width, center.y - half_height, 0.0f));

	model->positions.push_back(vec3(center.x + half_width, center.y + half_height, 0));
	model->positions.push_back(vec3(center.x + half_width + right_size, center.y + half_height + top_size, 0));
	model->positions.push_back(vec3(center.x + half_width, center.y - half_height, 0));
	model->positions.push_back(vec3(center.x + half_width + right_size, center.y - half_height - bottom_size, 0));

	model->positions.push_back(vec3(center.x - half_width - left_size, center.y + half_height + top_size, 0));
	model->positions.push_back(vec3(center.x + half_width + right_size, center.y + half_height + top_size, 0));
	model->positions.push_back(vec3(center.x - half_width, center.y + half_height, 0));
	model->positions.push_back(vec3(center.x + half_width, center.y + half_height, 0));

	model->getPositions()->push_back(vec3(center.x - half_width, center.y - half_height, 0));
	model->getPositions()->push_back(vec3(center.x + half_width, center.y - half_height, 0));
	model->getPositions()->push_back(vec3(center.x - half_width - left_size, center.y - half_height - bottom_size, 0));
	model->getPositions()->push_back(vec3(center.x + half_width + right_size, center.y - half_height - bottom_size, 0));

	vec3 normal = vec3(0, 1, 0);
	model->normals.push_back(normal);
	model->normals.push_back(normal);
	model->normals.push_back(normal);
	model->normals.push_back(normal);

	model->normals.push_back(normal);
	model->normals.push_back(normal);
	model->normals.push_back(normal);
	model->normals.push_back(normal);

	model->normals.push_back(normal);
	model->normals.push_back(normal);
	model->normals.push_back(normal);
	model->normals.push_back(normal);

	model->normals.push_back(normal);
	model->normals.push_back(normal);
	model->normals.push_back(normal);
	model->normals.push_back(normal);

	model->texCoords.push_back(vec2(0, 0));	// 0
	model->texCoords.push_back(vec2(1, 0));	// 1
	model->texCoords.push_back(vec2(0, 1));	// 2
	model->texCoords.push_back(vec2(1, 1));	// 3

	model->texCoords.push_back(vec2(0, 0));	// 0
	model->texCoords.push_back(vec2(1, 0));	// 1
	model->texCoords.push_back(vec2(0, 1));	// 2
	model->texCoords.push_back(vec2(1, 1));	// 3

	model->texCoords.push_back(vec2(0, 0));	// 0
	model->texCoords.push_back(vec2(1, 0));	// 1
	model->texCoords.push_back(vec2(0, 1));	// 2
	model->texCoords.push_back(vec2(1, 1));	// 3

	model->texCoords.push_back(vec2(0, 0));	// 0
	model->texCoords.push_back(vec2(1, 0));	// 1
	model->texCoords.push_back(vec2(0, 1));	// 2
	model->texCoords.push_back(vec2(1, 1));	// 3

	model->indices.push_back(0);
	model->indices.push_back(3);
	model->indices.push_back(1);
	model->indices.push_back(0);
	model->indices.push_back(2);
	model->indices.push_back(3);

	model->indices.push_back(0 + 4);
	model->indices.push_back(3 + 4);
	model->indices.push_back(1 + 4);
	model->indices.push_back(0 + 4);
	model->indices.push_back(2 + 4);
	model->indices.push_back(3 + 4);

	model->indices.push_back(0 + 8);
	model->indices.push_back(3 + 8);
	model->indices.push_back(1 + 8);
	model->indices.push_back(0 + 8);
	model->indices.push_back(2 + 8);
	model->indices.push_back(3 + 8);

	model->indices.push_back(0 + 12);
	model->indices.push_back(3 + 12);
	model->indices.push_back(1 + 12);
	model->indices.push_back(0 + 12);
	model->indices.push_back(2 + 12);
	model->indices.push_back(3 + 12);

	genVAO();
	genVBOS();
	genIndexBuffer();
}

IndexedModel* Border::getIndexedModel()
{
	return model;
}