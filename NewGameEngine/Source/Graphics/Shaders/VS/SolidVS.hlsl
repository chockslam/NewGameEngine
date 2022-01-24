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
   // float3 musParams : MusParams;
};

VSOut main(float3 pos : Position)
{
    VSOut vso;
    vso.viewPos = mul(float4(pos, 1.0f), modelViewProj);
   // vso.musParams = musparams;
	return vso;
}