Texture2D texture0;
SamplerState sampler0;
TextureCube skybox;

cbuffer CB0
{
	matrix WorldViewProjection; // 64 bytes
    float4 color; //16 bytes
}; // total = 80 bytes


struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

VOut VShader(float4 position: POSITION, float2 texcoord : TEXCOORD)
{
	VOut output;

	float4 default_color = { 1.0,1.0,1.0,1.0 };
	output.position = mul(WorldViewProjection, position);
	output.texcoord = position.xy;
	output.color = default_color;

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
    float distsq = texcoord.x * texcoord.x + texcoord.y * texcoord.y;
    clip(1.0f - distsq);
    return color;
}