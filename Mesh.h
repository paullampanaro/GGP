#pragma once

#include <d3d11.h>
#include "Vertex.h"
#include <vector>
#include <DirectXMath.h>
#include <fstream>

class Mesh
{
public:
	// constructor
	Mesh(Vertex* vertices, int numVertices, unsigned int * indices, int numIndices, ID3D11Device* device);
	Mesh(const char * file, ID3D11Device * device);
	~Mesh();

	// returns pointer to vertex buffer object
	ID3D11Buffer* GetVertexBuffer();

	// returns point to index buffer object
	ID3D11Buffer* GetIndexBuffer();

	// returns number of indices in this mesh
	int GetIndexCount();

private:
	// primary buffers
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// number of indices in the mesh's index buffer
	int numIndices;

	// buffer creation helper code
	void BuildBuffer(Vertex* vertices, int numVertices, unsigned int * indices, int numIndices, ID3D11Device* device);
};
