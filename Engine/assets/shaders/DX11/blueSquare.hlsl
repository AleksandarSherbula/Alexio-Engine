float4 VSMain(float2 inPos : aPosition) : SV_POSITION
{
    return float4(inPos, 0, 1);
}

float4 PSMain() : SV_TARGET
{
    return float4(0.0, 0.8, 1.0, 1.0);
}