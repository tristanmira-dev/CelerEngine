struct PushConst {
    uint32_t gameObjectIdx;
}

struct GameObjectProperties {
    float4x4 transform;
    uint32_t textureIdx;
}

struct ViewProperties {
    float4x4 projection;
    float4x4 model;
    float4x4 view;
}

[[vk::push_constant]]
PushConst pushConst;

[vk::binding(0, 0)]
Sampler2D sampler[20]; /*CHANGE THIS TO THE TOTAL AMOUNT OF ALLOWABLE TEXTURES LATER*/

[vk::binding(1, 0)]
StructuredBuffer<GameObjectProperties> gameObjectProperties;

[vk::binding(2, 0)]
ConstantBuffer<ViewProperties> viewProperties;

struct VSInput {
    float3 position;
    float3 color;
    float2 uv;
};

struct VertexOutput {
    float4 sv_position : SV_Position; /*just denotes whether to perform perspective divide on it or to be used in rasterization*/
    float3 color;
    float2 uv;
    int32_t textureIdx;
}

[shader("vertex")]
VertexOutput vertMain(VSInput input) {
    VertexOutput output;
    output.sv_position =  mul(viewProperties.projection, float4(input.position,1) );
    output.color = input.color;
    output.uv = input.uv;
    output.textureIdx = gameObjectProperties[pushConst.gameObjectIdx].textureIdx;
    return output;
}

[shader("fragment")]
float4 fragMain(VertexOutput vertIn) : SV_Target {
    return sampler[vertIn.textureIdx].Sample(vertIn.uv);
}