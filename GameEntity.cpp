#include "GameEntity.h"

GameEntity::GameEntity(Mesh * a_mesh)
{
	mesh = a_mesh;

	// default values
	XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());
	position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
}

// destructor, currently not very useful
GameEntity::~GameEntity()
{
}

// get methods
DirectX::XMFLOAT4X4 GameEntity::getWorldMatrix()
{
	return worldMatrix;
}

DirectX::XMFLOAT3 GameEntity::getPosition()
{
	return position;
}

DirectX::XMFLOAT3 GameEntity::getRotation()
{
	return rotation;
}

DirectX::XMFLOAT3 GameEntity::getScale()
{
	return scale;
}

// set methods
void GameEntity::setWorldMatrix(DirectX::XMFLOAT4X4 a_matrix)
{
	worldMatrix = a_matrix;
}

void GameEntity::setPosition(DirectX::XMFLOAT3 a_vector)
{
	position = a_vector;
}

void GameEntity::setRotation(DirectX::XMFLOAT3 a_vector)
{
	rotation = a_vector;
}

void GameEntity::setScale(DirectX::XMFLOAT3 a_vector)
{
	scale = a_vector;
}


