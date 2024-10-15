# Aiva DirectX12 Renderer

**Unlock the potential of DirectX12 with Aiva.** Aiva is a modern renderer built using C++ and WinRT, designed for high-performance graphics programming using Visual Studio 2022.

<p align="center">
  <img src="promo/aiva.jpg" style="max-width:100%; height:auto;"/>
</p>

## Features

- **DirectX12 Renderer:** Advanced graphics performance.
- **Modern C++ and WinRT:** Leverage the latest programming techniques.
- **Visual Studio 2022 Integration:** Optimized for modern development workflows.

## Usage

1. Open the project in Visual Studio 2022.
2. Build the solution and run the Aiva renderer.

## Code Examples

### Rendering a Triangle

Here's how you can render a triangle using Aiva:

```cpp
void deffered_renderer_t::tick_triangle()
{
    assert_t::check_bool(m_triangle_material, "(m_triangle_material) is not valid");

    auto const clear_render_target = std::make_shared<gpu_cmd_clear_render_target_t>(get_engine());
    assert_t::check_bool(clear_render_target, "(clear_render_target) is not valid");
    (*clear_render_target).set_render_target_ptr(get_engine().get_graphic_renderer().get_screen_target_ptr());
    (*clear_render_target).set_clear_color_opt(glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });

    get_engine().get_graphic_renderer().execute_render_command(clear_render_target);

    auto const set_render_target = std::make_shared<gpu_cmd_set_render_target_t>(get_engine());
    assert_t::check_bool(set_render_target, "(set_render_target) is not valid");
    (*set_render_target).set_render_target_ptr(get_engine().get_graphic_renderer().get_screen_target_ptr());

    get_engine().get_graphic_renderer().execute_render_command(set_render_target);

    auto const draw_proc_command = std::make_shared<gpu_cmd_draw_proc_t>(get_engine());
    assert_t::check_bool(draw_proc_command, "(draw_proc_command) is not valid");
    (*draw_proc_command).set_material_ptr(m_triangle_material);
    (*draw_proc_command).set_vertices_count(3);
    (*draw_proc_command).set_instances_count(1);

    get_engine().get_graphic_renderer().execute_render_command(draw_proc_command);
}
```

### Shader Example

An example of a shader using Aiva's syntax:

```hlsl
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
```

## Feedback and Support

Your feedback drives improvements. Submit issues on GitHub.
