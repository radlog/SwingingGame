Texture2D texture0;
SamplerState sampler0;

cbuffer CBuffer0
{
    float4x4 WVPMatrix; // 64 bytes
}; // total  64 bytes


struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;


    output.position = mul(WVPMatrix, position);
    output.color = color;

    return output;
}

// NOTE: try to change the name to see if it still works
float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{

    return color;
}