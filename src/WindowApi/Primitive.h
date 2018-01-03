#pragma once
#include "IndexedModel.h"
#include <GL/glew.h>

#define ANGLE 30.0f

class Primitive
{
public:
	Primitive();
	virtual ~Primitive();
};

class Quad : public Primitive
{
public:
	Quad(hm::vec3 center, float sizeX, float sizeY);
	Quad(hm::vec3& topLeftCorner, hm::vec3& bottomRightCorner);
	Quad(hm::vec3 center, float sizeX, float sizeY, int textureNumRows, int textureIndex);
	virtual ~Quad();
	IndexedModel* getIndexedModel();

	float getTextureIndex();
	float getTextureNumRows();
	hm::vec2& getTextureOffset();
	void updateQuad(int width, int height);

	void setIndex(int i);
	float getIndex();
private:
	float index;
	float numRows;
	hm::vec2 offset;
	hm::vec3 m_center;

	void calcAtlas();

	IndexedModel* model;
};

class Border : public Primitive
{
public:
	Border(hm::vec3 center, float width, float height, float left_size, float right_size, float top_size, float bottom_size);

	GLuint VertexArrayID;
	GLuint VertexBufferID;
	GLuint NormalsBufferID;
	GLuint TextureBufferID;
	GLuint IndexBufferID;

	IndexedModel* model;	// left right top bottom
private:
	hm::vec4 bg_color;

	void genVAO();
	void genVBOS();
	void genIndexBuffer();

public:
	inline hm::vec4 getBGColor() { return bg_color; }

	inline void setBGColor(hm::vec4 color) { bg_color = color; }
	IndexedModel* getIndexedModel();
};