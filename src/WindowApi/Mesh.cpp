#include "Mesh.h"
//#include "Grid.h"
#include "Primitive.h"
#include "IndexedModel.h"

bool Mesh::wireframe = false;
bool Mesh::isGUI = false;

Mesh::Mesh(Quad* quad) : Mesh(quad, false){}

Mesh::Mesh(Quad* quad, bool dynamicDraw)
{
	this->indexedModel = quad->getIndexedModel();
	this->quad = quad;
	this->reflectivity = 0;
	this->glossiness = 0;
	if (dynamicDraw)
		this->openglDrawHint = GL_DYNAMIC_DRAW;
	else
		this->openglDrawHint = GL_STATIC_DRAW;

	genVAO();
	genVBOS(quad->getIndexedModel());
	indicesSize = quad->getIndexedModel()->indices.size() * sizeof(float);
	genIndexBuffer(quad->getIndexedModel());

	this->referenceCount = 0;
}

Mesh::~Mesh()
{
	if (VertexArrayID != NULL)
		glDeleteVertexArrays(1, &VertexArrayID);
	if (VertexBufferID != NULL)
		glDeleteBuffers(1, &VertexBufferID);
	if (NormalsBufferID != NULL)
		glDeleteBuffers(1, &NormalsBufferID);
	if (TextureBufferID != NULL)
		glDeleteBuffers(1, &TextureBufferID);
	if (IndexBufferID != NULL)
		glDeleteBuffers(1, &IndexBufferID);

	if (quad != NULL)
		delete quad;
	if (indexedModel != nullptr && quad == NULL)
	{
		delete indexedModel;
	}
}

void Mesh::updateQuad() 
{
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, quad->getIndexedModel()->positions.size() * sizeof(float) * 3, &quad->getIndexedModel()->positions[0], openglDrawHint);
	glDisableVertexAttribArray(0);
}

void Mesh::genVAO()
{
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
}

void Mesh::genVBOS(IndexedModel* iModel)
{
	// Vertex Data
	glGenBuffers(1, &VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, iModel->positions.size() * sizeof(float) * 3, &iModel->positions[0], openglDrawHint);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDisableVertexAttribArray(0);

	// Normals Data
	glGenBuffers(1, &NormalsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, NormalsBufferID);
	glBufferData(GL_ARRAY_BUFFER, iModel->normals.size() * sizeof(float) * 3, &iModel->normals[0], openglDrawHint);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDisableVertexAttribArray(1);

	// Texture Data
	glGenBuffers(1, &TextureBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, TextureBufferID);
	glBufferData(GL_ARRAY_BUFFER, iModel->texCoords.size() * sizeof(float) * 2, &iModel->texCoords[0], openglDrawHint);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glDisableVertexAttribArray(2);
}

void Mesh::genIndexBuffer(IndexedModel* iModel)
{
	glGenBuffers(1, &IndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, &iModel->indices[0], GL_STATIC_DRAW);
}

void Mesh::render()
{
	glBindVertexArray(VertexArrayID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	if (wireframe && !isGUI)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, indicesSize / sizeof(float), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

Quad* Mesh::getQuad()
{
	if (quad == NULL)
		return NULL;
	else
		return quad;
}

int& Mesh::getReferenceCount()
{
	return this->referenceCount;
}

void Mesh::setIndexedModel(IndexedModel* im)
{
	this->indexedModel = im;
}

void Mesh::setReferenceCount(int refCount)
{
	this->referenceCount = refCount;
}

GLuint Mesh::getVertexArrayID()
{
	return VertexArrayID;
}
GLuint Mesh::getVertexBufferID()
{
	return VertexBufferID;
}
GLuint Mesh::getTextureBufferID()
{
	return TextureBufferID;
}
GLuint Mesh::getNormalsBufferID()
{
	return NormalsBufferID;
}
GLuint Mesh::getIndexBufferID()
{
	return IndexBufferID;
}
