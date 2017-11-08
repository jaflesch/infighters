#pragma once
#include <vector>
#include <hmath.h>
#include "..\ho_gl.h"

class Quad;
class IndexedModel;

class Mesh
{
public:
	Mesh(Quad* quad);
	Mesh(Quad* quad, bool dynamicDraw);
	~Mesh();
	void render();
	float reflectivity;
	float glossiness;
	static bool wireframe;
	static bool isGUI;

	// returns NULL if quad does not exist
	Quad* getQuad();

	GLuint getVertexArrayID();
	GLuint getVertexBufferID();
	GLuint getTextureBufferID();
	GLuint getNormalsBufferID();
	GLuint getIndexBufferID();

	int& getReferenceCount();
	void setReferenceCount(int refCount);
	void setIndexedModel(IndexedModel* im);
private:
	Quad* quad;
	IndexedModel* indexedModel;
	GLuint VertexArrayID;
	GLuint VertexBufferID;
	GLuint TextureBufferID;
	GLuint NormalsBufferID;
	GLuint IndexBufferID;

	std::vector<unsigned int> indices;

	void genVAO();
	void genVBOS(IndexedModel*);
	void genIndexBuffer(IndexedModel*);

	int indicesSize;
	int openglDrawHint;

	int referenceCount;
};



