//cbuffer CBuf
//{
//	matrix modelView;
//	matrix modelViewProj;
//};

//cbuffer musData
//{
//    float3 musparams;
//};

// Just passing data to the Geometry shader without applying any transformation 
struct VSOut
{
    float4 viewPos : SV_Position;
    float3 vNormal : NORMAL;
};

VSOut main(float3 pos : Position, float3 n: Normal)
{
    VSOut vso;
    vso.viewPos = float4(pos, 1.0f);
    vso.vNormal = n;
	return vso;
}