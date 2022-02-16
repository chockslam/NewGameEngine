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




[maxvertexcount(12)]
void main(triangle GSOut input[3] : SV_POSITION, inout TriangleStream<GSOut> OutputStream)
{
    GSOut gsout1;
    
    float3 MusParams = params;
    
    if(params.x == 0.0)
        MusParams.x = 1.0;
    if(params.y == 0.0)
        MusParams.y = 1.0;
    if(params.z == 0.0)
        MusParams.z = 1.0;
    
    MusParams = normalize(MusParams);
    
    
    float4 barycenter = (input[0].pos + input[1].pos + input[2].pos) / 3;
    float3 normal = (input[0].vNormal + input[1].vNormal + input[2].vNormal) / 3;
    float _ExtrusionFactor = MusParams.x;
    for (int i = 0; i < 3; i++)
    {
        int next = (i + 1) % 3;
        gsout1.pos = mul(float4(input[i].pos.x, input[i].pos.y, input[i].pos.z, input[i].pos.w), modelViewProj);
        gsout1.vNormal = mul(float3(input[i].vNormal.x, input[i].vNormal.y, input[i].vNormal.z), (float3x3) modelView);
        OutputStream.Append(gsout1);
        gsout1.pos = mul(barycenter + float4(normal, 0.0) * _ExtrusionFactor, modelViewProj);
        gsout1.vNormal = mul(float3(input[i].vNormal.x, input[i].vNormal.y, input[i].vNormal.z), (float3x3) modelView);
        OutputStream.Append(gsout1);
        gsout1.pos = mul(float4(input[next].pos.x, input[next].pos.y, input[next].pos.z, input[next].pos.w), modelViewProj);
        gsout1.vNormal = mul(float3(input[i].vNormal.x, input[i].vNormal.y, input[i].vNormal.z), (float3x3) modelView);
        OutputStream.Append(gsout1);
        
        OutputStream.RestartStrip();
    }
    for (int i = 0; i < 3; i++)
    {
        gsout1.pos = mul(float4(input[i].pos.x, input[i].pos.y, input[i].pos.z, input[i].pos.w), modelViewProj);
        //UNITY_TRANSFER_FOG(o, o.vertex);
       // o.uv = TRANSFORM_TEX(IN[i].uv, _MainTex);
        //o.color = fixed4(0.0, 0.0, 0.0, 1.0);
        OutputStream.Append(gsout1);
    }
    OutputStream.RestartStrip();

}

/*

struct GSOut
{
    float4 pos : SV_POSITION;
};

cbuffer CBuf
{
    float3 color;
};

[maxvertexcount(5)]
void main(point GSOut input[1] : SV_POSITION, inout TriangleStream<GSOut> OutputStream)
{
    GSOut gsout1;
    //GSOut gsout2;
    
    //for (int i = 0; i < 1; i++)
    //{
        gsout1.pos = float4(input[0].pos.x + 0.02 * color.x, input[0].pos.y, input[0].pos.z, input[0].pos.w);
        OutputStream.Append(gsout1);
        gsout1.pos = float4(input[0].pos.x - 0.02 * color.x, input[0].pos.y, input[0].pos.z, input[0].pos.w);
        OutputStream.Append(gsout1);
        gsout1.pos = float4(input[0].pos.x, input[0].pos.y + 0.02 * color.y, input[0].pos.z, input[0].pos.w);
        OutputStream.Append(gsout1);
        gsout1.pos = float4(input[0].pos.x, input[0].pos.y - 0.02 * color.y, input[0].pos.z, input[0].pos.w);
        OutputStream.Append(gsout1);
        gsout1.pos = float4(input[0].pos.x, input[0].pos.y, input[0].pos.z + 0.3 * color.z, input[0].pos.w);
        OutputStream.Append(gsout1);
        //gsout2.pos = float4(input[i].pos.x - 0.02 * i * color.z, input[i].pos.y - 0.02 * i * color.y, input[i].pos.z, input[i].pos.w);
        //OutputStream.Append(gsout2);
    //}
}
*/