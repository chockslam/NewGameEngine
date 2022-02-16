cbuffer CBuf
{
    matrix modelView;
    matrix modelViewProj;
};

struct GSOut
{
    float4 pos : SV_POSITION;
};

[maxvertexcount(4)]
void main(triangle GSOut input[3] : SV_POSITION, inout TriangleStream<GSOut> OutputStream)
{
    GSOut gsout;
    
    for (int i = 0; i < 3; i++)
    {
        gsout.pos = mul(float4(input[i].pos.x, input[i].pos.y, input[i].pos.z, input[i].pos.w),modelViewProj);
        OutputStream.Append(gsout);
    }
    
    OutputStream.RestartStrip();
    
}