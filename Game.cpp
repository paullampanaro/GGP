#include "Game.h"
#include "WICTextureLoader.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore( 
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	//vertexBuffer = 0;
	// indexBuffer = 0;
	vertexShader = 0;
	pixelShader = 0;

	// initialize mesh
	mesh1 = 0;
	mesh2 = 0;
	mesh3 = 0;

	// initialize material
	mat1 = 0;

	// initialize camera
	camera = new Camera();

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Release any (and all!) DirectX objects
	// we've made in the Game class
	//if (vertexBuffer) { vertexBuffer->Release(); }
	//if (indexBuffer) { indexBuffer->Release(); }

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;

	// delete mesh pointers
	delete mesh1;
	delete mesh2;
	delete mesh3;

	// delete camera
	delete camera;

	// delete material
	delete mat1;

	// release textures
	plasterSRV->Release();
	rustpaintSRV->Release();
	sampler->Release();
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();

	// load textures
	CreateWICTextureFromFile(device, context, L"Assets/Textures/plaster.jpg", 0, &plasterSRV);
	CreateWICTextureFromFile(device, context, L"Assets/Textures/rustpaint.jpg", 0, &rustpaintSRV);

	// create sampler
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &sampler);

	// load shaders into material
	mat1 = new Material(vertexShader, pixelShader, plasterSRV, sampler);

	// initialize game entities
	gameEntities.push_back(GameEntity(mesh1, mat1));
	gameEntities.push_back(GameEntity(mesh2, mat1));
	gameEntities.push_back(GameEntity(mesh3, mat1));

	// initialize light struct
	light = 
	{
		XMFLOAT4(0.1, 0.1, 0.1, 1.0), // ambient color
		XMFLOAT4(0, 0, 1, 1), // diffuse color
		XMFLOAT3(1, -1, 0) // direction
	};

	light2 =
	{
		XMFLOAT4(0.1, 0.1, 0.1, 1.0), // ambient color
		XMFLOAT4(1, 0 , 0 ,1), // diffuse color
		XMFLOAT3(1, 0 , -1) // direction
	};

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(L"Debug/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"VertexShader.cso");		

	pixelShader = new SimplePixelShader(device, context);
	if(!pixelShader->LoadShaderFile(L"Debug/PixelShader.cso"))	
		pixelShader->LoadShaderFile(L"PixelShader.cso");

	// You'll notice that the code above attempts to load each
	// compiled shader file (.cso) from two different relative paths.

	// This is because the "working directory" (where relative paths begin)
	// will be different during the following two scenarios:
	//  - Debugging in VS: The "Project Directory" (where your .cpp files are) 
	//  - Run .exe directly: The "Output Directory" (where the .exe & .cso files are)

	// Checking both paths is the easiest way to ensure both 
	// scenarios work correctly, although others exist
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//   update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//   an identity matrix.  This is just to show that HLSL expects a different
	//   matrix (column major vs row major) than the DirectX Math library
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up  = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V   = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//   the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!

	// create projection matrix of camera class
	camera->ManageProjMatrix(width, height);
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	// normals
	XMFLOAT3 normal = XMFLOAT3(0, 0, -1);

	// uv
	XMFLOAT2 uv = XMFLOAT2(0, 0);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	Vertex vertices[] = 
	{
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f), normal, uv },
		{ XMFLOAT3(+1.5f, -1.0f, +0.0f), normal, uv },
		{ XMFLOAT3(-1.5f, -1.0f, +0.0f), normal, uv },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	UINT indices[] = { 0, 1, 2 };

	// create the meshes
	// mesh1 = new Mesh(vertices, 3, indices, 3, device);
	mesh1 = new Mesh("Assets/Models/cylinder.obj", device);
	mesh2 = new Mesh("Assets/Models/cone.obj", device);
	mesh3 = new Mesh("Assets/Models/helix.obj", device);
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!

	// update projection matrix of camera on resize
	camera->ManageProjMatrix(width, height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
	
	// update camera - trying to use camera here cause "ambiguous errors"
	camera->Update(deltaTime);

	// update game entity position
	for (int i = 0; i < gameEntities.size(); i++)
	{
		// load the transformation vectors into DirectX Vectors 
		XMFLOAT3 tempPosition = gameEntities[i].GetPosition();
		XMFLOAT3 tempRotation = gameEntities[i].GetRotation();
		XMFLOAT3 tempScale = gameEntities[i].GetScale();
		XMFLOAT4X4 tempWorldMatrix;

		XMVECTOR v_position = XMLoadFloat3(&tempPosition);
		XMVECTOR v_rotation = XMLoadFloat3(&tempRotation);
		XMVECTOR v_scale = XMLoadFloat3(&tempScale);

		// DO WORK HERE
		if (i == 0)
		{
			XMVECTOR move = XMLoadFloat3(&XMFLOAT3(1.0f * deltaTime, 0.0f, 0.0f));
			v_position = v_position + move;
		}

		if (i == 1)
		{
			XMVECTOR move = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 1.0f * deltaTime));
			v_rotation = v_rotation + move;
		}

		/*
		if (i == 2)
		{
			float scale = tempScale.x + deltaTime;
			v_scale = XMLoadFloat3(&XMFLOAT3(scale, scale, scale));
		}
		*/

		// make the matrix
		XMMATRIX m_worldMatrix = XMMatrixScalingFromVector(v_scale) * XMMatrixRotationRollPitchYawFromVector(v_rotation) * XMMatrixTranslationFromVector(v_position);

		// store all values back into class
		XMStoreFloat3(&tempPosition, v_position);
		XMStoreFloat3(&tempRotation, v_rotation);
		XMStoreFloat3(&tempScale, v_scale);
		XMStoreFloat4x4(&tempWorldMatrix, XMMatrixTranspose(m_worldMatrix));

		gameEntities[i].SetPosition(tempPosition);
		gameEntities[i].SetRotation(tempRotation);
		gameEntities[i].SetScale(tempScale);
		gameEntities[i].SetWorldMatrix(tempWorldMatrix);
	}
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// pass each entity's world matrix to the shader before drawing
	for (int i = 0; i < gameEntities.size(); i++)
	{
		/*
		// Send data to shader variables
		//  - Do this ONCE PER OBJECT you're drawing
		//  - This is actually a complex process of copying data to a local buffer
		//    and then copying that entire buffer to the GPU.  
		//  - The "SimpleShader" class handles all of that for you.
		vertexShader->SetMatrix4x4("world", gameEntities[i].GetWorldMatrix());
		vertexShader->SetMatrix4x4("view", camera->GetViewMatrix());
		vertexShader->SetMatrix4x4("projection", camera->GetProjMatrix());

		// Once you've set all of the data you care to change for
		// the next draw call, you need to actually send it to the GPU
		//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
		vertexShader->CopyAllBufferData();

		// Set the vertex and pixel shaders to use for the next Draw() command
		//  - These don't technically need to be set every frame...YET
		//  - Once you start applying different shaders to different objects,
		//    you'll need to swap the current shaders before each draw
		vertexShader->SetShader();
		pixelShader->SetShader();
		*/

		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		// storing result in a variable
		ID3D11Buffer* vBuffer = gameEntities[i].GetMesh()->GetVertexBuffer();
		ID3D11Buffer* iBuffer = gameEntities[i].GetMesh()->GetIndexBuffer();

		context->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
		context->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0);

		pixelShader->SetData(
			"light", // name of variable in shader
			&light, // address of data to copy
			sizeof(DirectionalLight) // size of data to copy
		);

		pixelShader->SetData(
			"light2",
			&light2,
			sizeof(DirectionalLight)
		);

		// pixelShader
		// do all that in here instead
		gameEntities[i].PrepareMaterials(camera->GetViewMatrix(), camera->GetProjMatrix());

		// Finally do the actual drawing
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		context->DrawIndexed(
			gameEntities[i].GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	camera->MouseUpdate(prevMousePos.x - x, prevMousePos.y - y);

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion