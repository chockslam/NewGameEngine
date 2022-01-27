struct GSOut
{
    float4 pos : SV_POSITION;
};

[maxvertexcount(4)]
GSOut main(point float4 input[1] : SV_POSITION, inout TriangleStream<GSOut> OutputStream)
{
    GSOut gsout;
    gsout.pos = float4(input[0].x + 0.5, input[0].y + 0.5, input[0].z, input[0].w);
    OutputStream.Append(gsout);
    return gsout;
}