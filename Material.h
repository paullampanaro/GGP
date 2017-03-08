#pragma once

#include "SimpleShader.h"

class Material
{
private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	ID3D11ShaderResourceView* texture;
	ID3D11SamplerState* sampler;

public:
	// constructor & destructor
	Material(SimpleVertexShader* a_vShader, SimplePixelShader* a_pShader, ID3D11ShaderResourceView* a_texture, ID3D11SamplerState* a_sampler);
	~Material();

	// get methods
	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();
	ID3D11ShaderResourceView* GetTexture();
	ID3D11SamplerState* GetSampler();
};