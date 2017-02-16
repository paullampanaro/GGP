#include "Camera.h"

Camera::Camera()
{
	rotationX = 0;
	rotationY = 0;
	position = DirectX::XMFLOAT3(0, 0, -5);
	direction = DirectX::XMFLOAT3(0, 0, 1);
}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
	// calculate current direction
	DirectX::XMVECTOR first = DirectX::XMQuaternionRotationRollPitchYaw(rotationX, rotationY, 0);
	DirectX::XMVECTOR second = DirectX::XMVectorSet(0, 0, 1, 0);
	DirectX::XMVECTOR v_direction = DirectX::XMVector3Rotate(first, second);

	// create new view matrix
	DirectX::XMVECTOR v_position = XMLoadFloat3(&position);
	DirectX::XMVECTOR v_up = DirectX::XMVectorSet(0, 1, 0, 0);
	DirectX::XMMATRIX m_view = DirectX::XMMatrixLookToLH(v_position, v_direction, v_up);

	// store into current view matrix
	DirectX::XMFLOAT4X4 view;
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(m_view));

	/*
	// keyboard input
	if (GetAsyncKeyState('W') * 0x8000)
	{
		v_position += v_direction * deltaTime;
	}
	if (GetAsyncKeyState('S') * 0x8000)
	{
		v_position += -v_direction * deltaTime;
	}
	if (GetAsyncKeyState('A') * 0x8000)
	{
		v_position += DirectX::XMVector3Cross(v_direction, v_up) * deltaTime;
	}
	if (GetAsyncKeyState('D') * 0x8000)
	{
		v_position += -XMVector3Cross(v_direction, v_up) * deltaTime;
	}
	if (GetAsyncKeyState(VK_SPACE) * 0x8000)
	{
		v_position += v_up * deltaTime;
	}
	*/


	// store position back
	XMStoreFloat3(&position, v_position);
}

// get methods
DirectX::XMFLOAT4X4 Camera::getViewMatrix()
{
	return viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::getProjMatrix()
{
	return projMatrix;
}
