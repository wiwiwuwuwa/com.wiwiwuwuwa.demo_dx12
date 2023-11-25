#aiva init_meta_block
    entry_for_vert = vert
    entry_for_frag = frag
    render_target_format = [ R8G8B8A8_UNORM ]
    depth_stencil_format = UNKNOWN
#aiva shut_meta_block

#aiva init_code_block
// init structures ---------------------------------------
struct a2v_t
{
    uint vertex_id : SV_VERTEXID;
};

struct v2f_t
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

struct f2a_t
{
    float4 color : SV_TARGET;
};
// shut structures ---------------------------------------

// init functions ----------------------------------------
v2f_t vert(a2v_t a2v)
{
    float2 pos[3] = { float2(-0.5, -0.5), float2(0.0, 0.5), float2(0.5, -0.5) };
    float3 col[3] = { float3(1.0, 0.0, 0.0), float3(0.0, 1.0, 0.0), float3(0.0, 0.0, 1.0) };

    v2f_t v2f;
    v2f.position = float4(pos[a2v.vertex_id], 0.5, 1.0);
    v2f.color = col[a2v.vertex_id];
    return v2f;
}

f2a_t frag(v2f_t v2f)
{
    f2a_t f2a;
    f2a.color = float4(v2f.color, 1.0);
    return f2a;
}
// shut functions ----------------------------------------
#aiva shut_code_block
