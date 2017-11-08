#pragma once
#include <hmath.h>
#include <vector>
#include <string>

struct ObjIndex{
public:
	int vertexIndex;
	int texCoordIndex;
	int normalIndex;
};

class IndexedModel
{
public:
	std::vector<hm::vec3>* getPositions();
	std::vector<hm::vec2>* getTexCoords();
	std::vector<hm::vec3>* getNormals();
	std::vector<unsigned int>* getIndices();
	//private:
	std::vector<hm::vec3> positions;
	std::vector<hm::vec2> texCoords;
	std::vector<hm::vec3> normals;
	std::vector<unsigned int> indices;
};