cbuffer CBuf
{
	matrix modelView;
	matrix modelViewProj;
};

//cbuffer musData
//{
//    float3 musparams;
//};

struct VSOut
{
    float4 viewPos : SV_Position;
};

VSOut main(float3 pos : Position)
{
    VSOut vso;
    vso.viewPos = mul(float4(pos, 1.0f), modelViewProj);
	return vso;
}