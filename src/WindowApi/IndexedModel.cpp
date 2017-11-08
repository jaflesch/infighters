#include "IndexedModel.h"
#include <fstream>
#include "..\common.h"

using namespace std;

vector<hm::vec3>* IndexedModel::getPositions()
{
	return &positions;
}
vector<hm::vec2>* IndexedModel::getTexCoords()
{
	return &texCoords;
}
vector<hm::vec3>* IndexedModel::getNormals()
{
	return &normals;
}
vector<unsigned int>* IndexedModel::getIndices()
{
	return &indices;
}