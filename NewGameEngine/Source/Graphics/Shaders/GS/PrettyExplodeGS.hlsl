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
    float3 direction = normal * ((5 * sin(mus.x) + 1.0) * 10);
    return position + float4(direction, 1.0);
}


[maxvertexcount(12)]
void main(triangle GSOut input[3] : SV_POSITION, inout TriangleStream<GSOut> OutputStream)
{
    GSOut gsout1;
    
    float3 MusParams = params;
    
    if (params.x == 0.0)
        MusParams.x = 0.1;
    if (params.y == 0.0)
        MusParams.y = 0.1;
    if (params.z == 0.0)
        MusParams.z = 0.1;
    
    MusParams = normalize(MusParams);
    
    
    float4 vector0 = input[0].pos - input[1].pos;
    float4 vector1 = input[2].pos - input[1].pos;
    float4 surfaceNormal = float4(normalize(cross(float3(vector0.x, vector0.y, vector0.z), float3(vector1.x, vector1.y, vector1.z))), 0.0f);
    
  //------ Generate a new face along the direction of the face normal
  // only if diff_len is not too small.
  //
    if (length(surfaceNormal) > 0.1)
    {
        int i;
        for (i = 0; i < 3; i++)
        {
            gsout1.pos = input[i].pos;
            float len = sqrt(input[i].pos.x * input[i].pos.x + input[i].pos.z * input[i].pos.z);
            float scale = 3.0 + 7 * cos(3+(MusParams.x*6.28) * 0.1 + len*2);
            gsout1.pos = float4(gsout1.pos + (surfaceNormal * length(surfaceNormal) * scale)) + float4(float3(surfaceNormal.x, surfaceNormal.y, surfaceNormal.z) * float3(0.05, 0.05, 0.05), 1.0);
            gsout1.pos = mul(gsout1.pos, modelViewProj);
            gsout1.vNormal = input[i].vNormal;
            OutputStream.Append(gsout1);

        }
        
        OutputStream.RestartStrip();
        
        
        for (i = 0; i < 3; i++)
        {
            gsout1.pos = input[i].pos + float4(7.5,0.0,0.0,0.0);
            float len = sqrt(input[i].pos.x * input[i].pos.x + input[i].pos.z * input[i].pos.z);
            float scale = 3.0 + 7 * cos(3 + (MusParams.y * 6.28) * 0.1 + len * 2);
            gsout1.pos = float4(gsout1.pos + (surfaceNormal * length(surfaceNormal) * scale)) + float4(float3(surfaceNormal.x, surfaceNormal.y, surfaceNormal.z) * float3(0.05, 0.05, 0.05), 1.0);
            gsout1.pos = mul(gsout1.pos, modelViewProj);
            gsout1.vNormal = input[i].vNormal;
            OutputStream.Append(gsout1);

        }
        
        OutputStream.RestartStrip();
        for (i = 0; i < 3; i++)
        {
            gsout1.pos = input[i].pos + float4(3.75,-10.5,0.0,0.0);
            float len = sqrt(input[i].pos.x * input[i].pos.x + input[i].pos.z * input[i].pos.z);
            
            float scale = 3.0 + 7 * cos(3 + (MusParams.z * 6.28) * 0.1 + len * 2);
        
            gsout1.pos = float4(gsout1.pos + (surfaceNormal * length(surfaceNormal) * scale)) + float4(float3(surfaceNormal.x, surfaceNormal.y, surfaceNormal.z) * float3(0.05, 0.05, 0.05), 1.0);
           
            gsout1.pos = mul(gsout1.pos, modelViewProj);
            gsout1.vNormal = input[i].vNormal;
            OutputStream.Append(gsout1);

        }
        
        OutputStream.RestartStrip();
        
        
        //for (i = 0; i < 3; i++)
        //{
        //    gsout1.pos = input[i].pos;
        //    float len = sqrt(input[i].pos.x * input[i].pos.x + input[i].pos.z * input[i].pos.z);
        //    float scale = 2.0 + 1.0 * cos(MusParams.y * 2.0 + len);
        //    gsout1.pos = float4(gsout1.pos + (surfaceNormal * length(surfaceNormal) * scale)) + float4(float3(surfaceNormal.x, surfaceNormal.y, surfaceNormal.z) * float3(0.05, 0.05, 0.05), 1.0);
        //    gsout1.pos = mul(gsout1.pos, modelViewProj);
        //    gsout1.vNormal = input[i].vNormal;
        //    OutputStream.Append(gsout1);
        //}
        //
        //OutputStream.RestartStrip();
        //for (i = 0; i < 3; i++)
        //{
        //    gsout1.pos = input[i].pos;
        //    float len = sqrt(input[i].pos.x * input[i].pos.x + input[i].pos.z * input[i].pos.z);
        //    float scale = 2.0 + 1.0 * cos(MusParams.z * 1.0 + len);
        //    gsout1.pos = float4(gsout1.pos + (surfaceNormal * length(surfaceNormal) * scale)) + float4(float3(surfaceNormal.x, surfaceNormal.y, surfaceNormal.z) * float3(0.05, 0.05, 0.05), 1.0);
        //    gsout1.pos = mul(gsout1.pos, modelViewProj);
        //    gsout1.vNormal = input[i].vNormal;
        //    OutputStream.Append(gsout1);
        //}
        //
        //OutputStream.RestartStrip();
    }
    

}
