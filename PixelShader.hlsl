
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	// float4 color		: COLOR;
	float3 normal		: NORMAL;
};

// directonal light
struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

// light buffer
cbuffer LightBuffer : register(b0)
{
	DirectionalLight light;
	DirectionalLight light2;
};

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	// normalize it
	input.normal = normalize(input.normal);

	// Calculate the normalized direction to the light
	// Negate the light’s direction, normalize that and store in a float3 variable
	// You can’t store it back in the light variable because it’s a constant
	float3 lightDir = normalize(-light.Direction);

	// Calculate the light amount using the N dot L equation
	// Use the dot(v1, v2) function with the surface’s normal and the direction to the light
	// The normal should already be normalized from a previous step
	// The dot product result can be negative, which will be problematic if we have multiple lights.Use the saturate() function to clamp your result between 0 and 1
	float lightAmount = saturate(dot(input.normal, lightDir));

	// redo for second light
	// will write helper fuction for this later
	float3 lightDir2 = normalize(-light2.Direction);
	float lightAmount2 = saturate(dot(input.normal, lightDir2));

	// Return the final surface color based on light amount, diffuse color and ambient color
	// Scale the light’s diffuse color by the light amount
	// Add the light’s ambient color
	// Return the result
	float4 finalSurface = (light.DiffuseColor * lightAmount + light.AmbientColor) + (light2.DiffuseColor * lightAmount2 + light2.AmbientColor);
	return finalSurface;

	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	// return float4(1,0,0,1);
}