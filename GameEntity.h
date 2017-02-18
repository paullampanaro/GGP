#pragma once

#include "Mesh.h"
#include "Material.h"

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

	// pointer to material object
	Material* mat;

public:
	//constructor
	GameEntity(Mesh* a_mesh, Material* a_mat);
	~GameEntity();

	// get methods
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMFLOAT3 GetScale();

	// set methods
	void SetWorldMatrix(DirectX::XMFLOAT4X4 a_matrix);
	void SetPosition(DirectX::XMFLOAT3 a_vector);
	void SetRotation(DirectX::XMFLOAT3 a_vector);
	void SetScale(DirectX::XMFLOAT3 a_vector);

	// material method
	void PrepareMaterials(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projMatrix);
};