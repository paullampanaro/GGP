#pragma once

#include "Mesh.h"

class GameEntity 
{
private:
	// world matrix
	DirectX::XMFLOAT4X4 worldMatrix;

	// transformations
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

	// pointer to mesh object
	Mesh* mesh;

public:
	//constructor
	GameEntity(Mesh* a_mesh);
	~GameEntity();

	// get methods
	DirectX::XMFLOAT4X4 getWorldMatrix();
	DirectX::XMFLOAT3 getPosition();
	DirectX::XMFLOAT3 getRotation();
	DirectX::XMFLOAT3 getScale();

	// set methods
	void setWorldMatrix(DirectX::XMFLOAT4X4 a_matrix);
	void setPosition(DirectX::XMFLOAT3 a_vector);
	void setRotation(DirectX::XMFLOAT3 a_vector);
	void setScale(DirectX::XMFLOAT3 a_vector);
};