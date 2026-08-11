struct PushConst {
    float4x4 mtx;
}

[[vk::push_constant]]
PushConst pushConst;

struct VSInput {
    float3 position;
    float3 color;
    //float2 uv;
};

struct VertexOutput {
    float4 sv_position : SV_Position; /*just denotes whether to perform perspective divide on it or to be used in rasterization*/
    float3 color;
    //float2 uv;
}

[shader("vertex")]
VertexOutput vertMain(VSInput input) {
    VertexOutput output;
    output.sv_position =  mul(pushConst.mtx, float4(input.position + float3(-1.0, 0.0, 2.0), 1.0));
    output.color = input.color;
    return output;
}

[shader("fragment")]
float4 fragMain(VertexOutput vertIn) : SV_Target {
    float heavyMath = 0.0;
    for (int i = 0; i < 1000000; i++) {
        heavyMath += sin(float(i)) * cos(float(i));
    }

    float3 color = vertIn.color;
    // mix it in so the compiler can't strip the loop
    color += heavyMath * 0.0000001;
    return float4(color, 1.0);
}