struct GSOut
{
    float4 pos : SV_POSITION;
    float3 vNormal : NORMAL;
};


cbuffer TransformCBuf
{
    matrix modelView;
    matrix modelViewProj;
};

cbuffer CBuf
{
    float3 params;
};

float4 explode(float4 position, float3 normal, float3 mus)
{
    float3 direction = normal * ((5*sin(mus.x) + 1.0) * 10);
    return position + float4(direction, 1.0);
}


[maxvertexcount(12)]
void main(triangle GSOut input[3] : SV_POSITION, inout TriangleStream<GSOut> OutputStream)
{
    GSOut gsout1;
    
    float3 MusParams = params;
    
    if (params.x == 0.0)
        MusParams.x = 1.0;
    if (params.y == 0.0)
        MusParams.y = 1.0;
    if (params.z == 0.0)
        MusParams.z = 1.0;
    
    MusParams = normalize(MusParams);
    
    
    float4 vector0 = input[0].pos - input[1].pos;
    float4 vector1 = input[2].pos - input[1].pos;
    float4 surfaceNormal = float4(normalize(cross(float3(vector0.x, vector0.y, vector0.z), float3(vector1.x, vector1.y, vector1.z))), 0.0f);
    for (int i = 0; i < 3; i++)
    {
        gsout1.vNormal = input[i].vNormal;
        gsout1.pos = mul(input[i].pos + surfaceNormal * (4 + MusParams.x/2), modelViewProj);
        OutputStream.Append(gsout1);
        
        //gsout1.pos = mul(input[i].pos + surfaceNormal * (3 + MusParams.y / 2), modelViewProj);
        //OutputStream.Append(gsout1);
        //
        //gsout1.pos = mul(input[i].pos + surfaceNormal * (2 + MusParams.z / 2), modelViewProj);
        //OutputStream.Append(gsout1);
        
    }
    OutputStream.RestartStrip();

}
