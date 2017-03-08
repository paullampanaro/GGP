#include "Material.h"

Material::Material(SimpleVertexShader * a_vShader, SimplePixelShader * a_pShader, ID3D11ShaderResourceView* a_texture, ID3D11SamplerState* a_sampler)
{
	vertexShader = a_vShader;
	pixelShader = a_pShader;
	texture = a_texture;
	sampler = a_sampler;
}

Material::~Material()
{
	delete vertexShader;
	delete pixelShader;
}

SimpleVertexShader * Material::GetVertexShader()
{
	return vertexShader;
}

SimplePixelShader * Material::GetPixelShader()
{
	return pixelShader;
}

ID3D11ShaderResourceView * Material::GetTexture()
{
	return texture;
}

ID3D11SamplerState * Material::GetSampler()
{
	return sampler;
}
