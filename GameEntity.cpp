#include "GameEntity.h"

GameEntity::GameEntity(Mesh * a_mesh, Material* a_mat)
{
	mesh = a_mesh;
	mat = a_mat;

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
DirectX::XMFLOAT4X4 GameEntity::GetWorldMatrix()
{
	return worldMatrix;
}

DirectX::XMFLOAT3 GameEntity::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 GameEntity::GetRotation()
{
	return rotation;
}

DirectX::XMFLOAT3 GameEntity::GetScale()
{
	return scale;
}

Mesh * GameEntity::GetMesh()
{
	return mesh;
}

// set methods
void GameEntity::SetWorldMatrix(DirectX::XMFLOAT4X4 a_matrix)
{
	worldMatrix = a_matrix;
}

void GameEntity::SetPosition(DirectX::XMFLOAT3 a_vector)
{
	position = a_vector;
}

void GameEntity::SetRotation(DirectX::XMFLOAT3 a_vector)
{
	rotation = a_vector;
}

void GameEntity::SetScale(DirectX::XMFLOAT3 a_vector)
{
	scale = a_vector;
}

// manage material class
void GameEntity::PrepareMaterials(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projMatrix)
{
	mat->GetVertexShader()->SetMatrix4x4("world", worldMatrix);
	mat->GetVertexShader()->SetMatrix4x4("view", viewMatrix);
	mat->GetVertexShader()->SetMatrix4x4("projection", projMatrix);

	mat->GetVertexShader()->CopyAllBufferData();
	mat->GetVertexShader()->SetShader();

	mat->GetPixelShader()->CopyAllBufferData();
	mat->GetPixelShader()->SetShader();
}


