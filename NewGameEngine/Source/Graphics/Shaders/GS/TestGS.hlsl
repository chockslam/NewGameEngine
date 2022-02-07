struct GSOut
{
    float4 pos : SV_POSITION;
};

cbuffer CBuf
{
    float3 color;
};

[maxvertexcount(8)]
void main(triangle GSOut input[3] : SV_POSITION, inout TriangleStream<GSOut> OutputStream)
{
    GSOut gsout1;
    GSOut gsout2;
    
    for (int i = 0; i < 3; i++)
    {
        gsout1.pos = float4(input[i].pos.x + 0.02 * i * color.x, input[i].pos.y + 0.02 * i * color.y, input[i].pos.z + 0.3 * i * color.z, input[i].pos.w);
        OutputStream.Append(gsout1);
        //gsout2.pos = float4(input[i].pos.x - 0.02 * i * color.z, input[i].pos.y - 0.02 * i * color.y, input[i].pos.z, input[i].pos.w);
        //OutputStream.Append(gsout2);
    }
}