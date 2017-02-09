#include "GameEntity.h"

GameEntity::GameEntity(Mesh * a_mesh)
{
	mesh = a_mesh;

	// default values
	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

// destructor, currently not very useful
GameEntity::~GameEntity()
{
}

// get methods
XMFLOAT4X4 GameEntity::getWorldMatrix()
{
	return worldMatrix;
}

XMFLOAT3 GameEntity::getPosition()
{
	return position;
}

XMFLOAT3 GameEntity::getRotation()
{
	return rotation;
}

XMFLOAT3 GameEntity::getScale()
{
	return scale;
}

// set methods
void GameEntity::setWorldMatrix(XMFLOAT4X4 a_matrix)
{
	worldMatrix = a_matrix;
}

void GameEntity::setPosition(XMFLOAT3 a_vector)
{
	position = a_vector;
}

void GameEntity::setRotation(XMFLOAT3 a_vector)
{
	rotation = a_vector;
}

void GameEntity::setScale(XMFLOAT3 a_vector)
{
	scale = a_vector;
}


