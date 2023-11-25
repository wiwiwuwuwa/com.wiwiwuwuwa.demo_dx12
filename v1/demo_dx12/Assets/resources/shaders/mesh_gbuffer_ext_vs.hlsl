// --------------------------------------------------------

struct t_input_data
{
    float3 position_os : POSITION;
    float3 normal_os: NORMAL;
    float2 texcoord_os : TEXCOORD0;

    uint vertex_id : SV_VertexID;
};

struct t_output_data
{
    float4 position_cs : SV_Position;
    float3 position_ws : TEXCOORD0;
    float3 normal_ws : TEXCOORD1;
    float2 texcoord_os : TEXCOORD2;
};

// --------------------------------------------------------

Texture2D<float4> tex_albedo : register(t0);

// --------------------------------------------------------

t_output_data main(t_input_data IN)
{    
    t_output_data OUT;
    OUT.position_cs = float4(IN.position_os, 1.0);
    OUT.position_ws = IN.position_os;
    OUT.normal_ws = IN.normal_os;
    OUT.texcoord_os = IN.texcoord_os;
    return OUT;
}

// --------------------------------------------------------
