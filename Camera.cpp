#include "Camera.h"

using namespace DirectX;

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
	DirectX::XMVECTOR first = DirectX::XMQuaternionRotationRollPitchYaw(rotationX, rotationY, 1);
	DirectX::XMVECTOR second = DirectX::XMVectorSet(0, 0, 1, 0);
	DirectX::XMVECTOR v_direction = DirectX::XMVector3Rotate(first, second);

	// create new view matrix
	DirectX::XMVECTOR v_position = XMLoadFloat3(&position);
	DirectX::XMVECTOR v_up = DirectX::XMVectorSet(0, 1, 0, 0);
	DirectX::XMMATRIX m_view = DirectX::XMMatrixLookToLH(v_position, v_direction, v_up);

	// store into current view matrix
	DirectX::XMFLOAT4X4 view;
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(m_view));

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
	if (GetAsyncKeyState('Z') * 0x8000)
	{
		v_position += v_up * deltaTime;
	}
	if (GetAsyncKeyState('X') * 0x8000)
	{
		v_position += -v_up * deltaTime;
	}


	// store values back
	XMStoreFloat3(&position, v_position);
}

// get methods
DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::GetProjMatrix()
{
	return projMatrix;
}

// rotate camera based on mouse movement
void Camera::MouseUpdate(int mouseX, int mouseY)
{
	// I couldn't figure this one out
	float limit = 0.2f;

	rotationX += mouseX * 0.01f;

	// limit
	if (rotationX > limit)
		rotationX = limit;
	if (rotationX < -limit)
		rotationX = -limit;

	// limit
	rotationY += mouseY * 0.01f;

	if (rotationY > limit)
		rotationY = limit;
	if (rotationY < -limit)
		rotationY = -limit;
}

void Camera::ManageProjMatrix(int a_width, int a_height)
{
	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//   the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)a_width / a_height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}
