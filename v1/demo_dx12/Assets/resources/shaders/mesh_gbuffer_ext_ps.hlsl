// --------------------------------------------------------

struct t_input_data
{
    float4 position_cs : SV_Position;
    float3 position_ws : TEXCOORD0;
    float3 normal_ws : TEXCOORD1;
    float2 texcoord_os : TEXCOORD2;
};

struct t_output_data
{
    float4 color: SV_Target0;
};

// --------------------------------------------------------

Texture2D<float4> tex_albedo : register(t0);

// --------------------------------------------------------

t_output_data main(t_input_data IN)
{
    t_output_data OUT;
    OUT.color = tex_albedo[uint2(0, 0)];
    return OUT;
}

// --------------------------------------------------------
