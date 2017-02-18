#pragma once

#include "Vertex.h"
#include "Windows.h"

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

	// get methods
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjMatrix();

	// mouse method
	void MouseUpdate(int mouseX, int mouseY);

	// projection matrix method
	void ManageProjMatrix(int a_width, int a_height);
};