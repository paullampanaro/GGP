#pragma once

#include "Mesh.h"
using namespace DirectX;


class GameEntity 
{
private:
	// world matrix
	XMFLOAT4X4 worldMatrix;

	// transformations
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;

	// pointer to mesh object
	Mesh* mesh;

public:
	//constructor
	GameEntity(Mesh* a_mesh);
	~GameEntity();

	// get methods
	XMFLOAT4X4 getWorldMatrix();
	XMFLOAT3 getPosition();
	XMFLOAT3 getRotation();
	XMFLOAT3 getScale();

	// set methods
	void setWorldMatrix(XMFLOAT4X4 a_matrix);
	void setPosition(XMFLOAT3 a_vector);
	void setRotation(XMFLOAT3 a_vector);
	void setScale(XMFLOAT3 a_vector);
};