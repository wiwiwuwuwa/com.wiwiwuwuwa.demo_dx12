#aiva init_meta_block
entry_for_comp = main
#aiva shut_meta_block

#aiva init_code_block
// init structures ---------------------------------------
struct input_t
{
    uint3 threads : SV_DISPATCHTHREADID;
};
// shut structures ---------------------------------------

// init variables ----------------------------------------
RWTexture2D<float4> m_main_texture : register(u0);
// shut variables ----------------------------------------

// init functions ----------------------------------------
[numthreads(16, 16, 1)]
void main(input_t input)
{
    uint2 texture_dimensions;
    m_main_texture.GetDimensions(texture_dimensions.x, texture_dimensions.y);

    if (input.threads.x >= texture_dimensions.x || input.threads.y >= texture_dimensions.y)
        return;

    uint2 cell_size = texture_dimensions / 4;

    uint checker_x = input.threads.x / cell_size.x;
    uint checker_y = input.threads.y / cell_size.y;
    float checker_value = (checker_x + checker_y) % 2;

    m_main_texture[input.threads.xy] = checker_value;
}
// shut functions ----------------------------------------
#aiva shut_code_block
