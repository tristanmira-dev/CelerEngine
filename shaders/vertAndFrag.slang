


struct VSInput {
    float3 position;
    float3 color;
    //float2 uv;
};

struct VertexOutput {
    float4 sv_position : SV_Position; /*just denotes the output, : is a specifier indicating its a special gpu property*/
    float3 color;
    //float2 uv;
}

[shader("vertex")]
VertexOutput vertMain(VSInput input) {
    VertexOutput output;
    output.sv_position = float4(input.position, 1.0);
    output.color = input.color;
    return output;
}

[shader("fragment")]
float4 fragMain(VertexOutput vertIn) : SV_Target {
    float3 color = vertIn.color;
    return float4(color, 1.0);
}