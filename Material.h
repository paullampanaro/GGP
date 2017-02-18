#pragma once

#include "SimpleShader.h"

class Material
{
private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

public:
	// constructor & destructor
	Material(SimpleVertexShader* vShader, SimplePixelShader* pShader);
	~Material();

	// get methods
	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();
};