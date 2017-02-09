#pragma once

#include <d3d11.h>
#include "Vertex.h"

class Mesh
{
public:
	// constructor
	Mesh(Vertex* vertices, int numVertices, int* indices, int numIndices, ID3D11Device* device);
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
};
