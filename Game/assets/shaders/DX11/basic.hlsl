cbuffer Camera : register(b0)
{
    float4x4 projection;
}

struct VS_INPUT
{
    float2 inPos : aPosition;
    float4 inColor : aColor;
};

struct VS_OUTPUT
{
    float4 outPos : SV_POSITION;
    float4 outColor : COLOR;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;
    output.outPos = mul(projection, float4(input.inPos, 0.0, 1.0));
    output.outColor = input.inColor;
    return output;
}

struct PS_INPUT
{
    float4 inPos : SV_POSITION;
    float4 inColor : COLOR;
};

float4 PSMain(PS_INPUT input) : SV_TARGET
{
    return input.inColor;
}
