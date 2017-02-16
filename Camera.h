#pragma once

#include "Vertex.h"

class Camera 
{
private:
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projMatrix;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 direction;

	float rotationX;
	float rotationY;

public:
	// constructor & destructor
	Camera();
	~Camera();

	// update
	void Update(float deltaTime);
	DirectX::XMFLOAT4X4 getViewMatrix();
	DirectX::XMFLOAT4X4 getProjMatrix();

};