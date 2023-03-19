cbuffer Camera : register(b0)
{
    float4x4 projection;
}

struct VS_INPUT
{
    float3 inPos : aPosition;
    float4 inColor : aColor;
    float2 inTexCoord : aTexCoord;
};

struct VS_OUTPUT
{
    float4 outPos : SV_POSITION;
    float4 outColor : COLOR;
    float2 outTexCoord : TEXCOORD;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;
    output.outPos = mul(projection, float4(input.inPos, 1.0));
    output.outColor = input.inColor;
    output.outTexCoord = input.inTexCoord;
    return output;
}

struct PS_INPUT
{
    float4 inPos : SV_POSITION;
    float4 inColor : COLOR;
    float2 inTexCoord : TEXCOORD;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 PSMain(PS_INPUT input) : SV_TARGET
{
    float4 pixel = objTexture.Sample(objSamplerState, input.inTexCoord) * input.inColor;
    return pixel;
}