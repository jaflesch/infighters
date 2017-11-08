#pragma once
#include "IndexedModel.h"

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

	void setIndex(int i);
	float getIndex();
private:
	float index;
	float numRows;
	hm::vec2 offset;

	void calcAtlas();

	IndexedModel* model;
};