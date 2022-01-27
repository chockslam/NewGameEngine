cbuffer CBuf
{
    float3 color;
};

float4 main() : SV_Target
{
    float3 NewColor = { color[0], color[2], color[1] };
    return float4(normalize(NewColor), 1.0f);
}