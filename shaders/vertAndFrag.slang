struct PushConst {
    float4x4 mtx;
}

[[vk::push_constant]]
PushConst pushConst;

[vk::binding(0, 0)]
Sampler2D sampler[1];

struct VSInput {
    float3 position;
    float3 color;
    float2 uv;
};

struct VertexOutput {
    float4 sv_position : SV_Position; /*just denotes whether to perform perspective divide on it or to be used in rasterization*/
    float3 color;
    float2 uv;
}

[shader("vertex")]
VertexOutput vertMain(VSInput input) {
    VertexOutput output;
    output.sv_position =  mul(pushConst.mtx, float4(input.position + float3(-7.0, 0.0, 2.0), 1.0));
    output.color = input.color;
    output.uv = input.uv;
    return output;
}

[shader("fragment")]
float4 fragMain(VertexOutput vertIn) : SV_Target {
    return sampler[0].Sample(vertIn.uv);
}