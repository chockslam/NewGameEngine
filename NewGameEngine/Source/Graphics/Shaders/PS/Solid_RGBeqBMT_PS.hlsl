// Mus Params
cbuffer CBuf
{
	float3 color;
};

float4 main() : SV_Target
{
    //output musparams as RGB colors 
    return float4(normalize(color), 1.0f);
}

//Shaders with the similar pattern of the name just put Bass, Mid, Treble to different RGB channels 