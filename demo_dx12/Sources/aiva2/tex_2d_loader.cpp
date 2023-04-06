#include <pch.h>
#include <aiva2/tex_2d_loader.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buf.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/gpu_cmd_copy_buf_to_tex_2d.hpp>
#include <aiva2/graphic_executor.hpp>
#include <aiva2/img_png.hpp>
#include <aiva2/resource_loader_registr.hpp>
#include <aiva2/resource_system.hpp>
#include <aiva2/tex_2d.hpp>

namespace aiva2
{
    REGISTER_RESOURCE_LOADER(tex_2d_loader_t);

    auto tex_2d_loader_t::load(engine_t& engine, nlohmann::json const& json) -> std::shared_ptr<tex_2d_t>
    {
        assert_t::check_bool(!std::empty(json), "(json) is not valid");

        auto const img_res = load_image(engine, json);
        assert_t::check_bool(img_res, "(img_res) is not valid");

        auto const tex_res = create_texture(engine, json, img_res);
        assert_t::check_bool(tex_res, "(tex_res) is not valid");

        copy_image_to_texture(engine, img_res, tex_res);

        return tex_res;
    }

    auto tex_2d_loader_t::load_image(engine_t& engine, nlohmann::json const& json)->std::shared_ptr<img_png_t>
    {
        assert_t::check_bool(!std::empty(json), "(json) is not valid");

        auto const img_pth = json.at("data_path").get<std::filesystem::path>();
        assert_t::check_bool(!std::empty(img_pth), "(img_pth) is not valid");

        auto const img_res = engine.get_resource_system().get_resource<img_png_t>(img_pth);
        assert_t::check_bool(img_res, "(img_res) is not valid");

        return img_res;
    }

    auto tex_2d_loader_t::create_texture(engine_t& engine, nlohmann::json const& json, std::shared_ptr<img_png_t> const& img_res)->std::shared_ptr<tex_2d_t>
    {
        assert_t::check_bool(!std::empty(json), "(json) is not valid");
        assert_t::check_bool(img_res, "(img_res) is not valid");

        auto const tex_inf = create_texture_info(json, img_res);
        auto const tex_res = std::make_shared<tex_2d_t>(engine, tex_inf);
        assert_t::check_bool(tex_res, "(tex_res) is not valid");

        return tex_res;
    }

    auto tex_2d_loader_t::create_texture_info(nlohmann::json const& json, std::shared_ptr<img_png_t> const& img_res)->tex_2d_info_t
    {
        assert_t::check_bool(!std::empty(json), "(json) is not valid");
        assert_t::check_bool(img_res, "(img_res) is not valid");

        auto tex_inf = tex_2d_info_t{};
        tex_inf.set_format(buffer_format_t::R8G8B8A8_UNORM);
        tex_inf.set_width((*img_res).get_width());
        tex_inf.set_height((*img_res).get_height());
        tex_inf.set_support_mip_mappping(json.at("support_mip_mapping").get<bool>());
        tex_inf.set_support_shader_resource(json.at("support_shader_resource").get<bool>());
        tex_inf.set_support_shader_atomics(json.at("support_shader_atomics").get<bool>());
        tex_inf.set_support_render_target(json.at("support_render_target").get<bool>());
        tex_inf.set_support_depth_stencil(json.at("support_depth_stencil").get<bool>());
        tex_inf.set_support_unordered_access(json.at("support_unordered_access").get<bool>());

        return tex_inf;
    }

    auto tex_2d_loader_t::copy_image_to_texture(engine_t& engine, std::shared_ptr<img_png_t> const& img_res, std::shared_ptr<tex_2d_t> const& tex_res)->void
    {
        assert_t::check_bool(img_res, "(img_res) is not valid");
        assert_t::check_bool(tex_res, "(tex_res) is not valid");

        auto const buf_res = create_buffer(engine, img_res);
        assert_t::check_bool(buf_res, "(buf_res) is not valid");

        copy_image_to_buffer(img_res, buf_res);
        copy_buffer_to_texture(engine, buf_res, tex_res);
    }

    auto tex_2d_loader_t::create_buffer(engine_t& engine, std::shared_ptr<img_png_t> const& img_res)->std::shared_ptr<buf_t>
    {
        assert_t::check_bool(img_res, "(img_res) is not valid");
        assert_t::check_bool(!std::empty((*img_res).get_data()), "(*img_res).get_data() is not valid");

        auto const buf_inf = create_buffer_info(img_res);
        auto const buf_res = std::make_shared<buf_t>(engine, buf_inf);
        assert_t::check_bool(buf_res, "(buf_res) is not valid");

        return buf_res;
    }

    auto tex_2d_loader_t::create_buffer_info(std::shared_ptr<img_png_t> const& img_res)->buf_info_t
    {
        assert_t::check_bool(img_res, "(img_res) is not valid");
        assert_t::check_bool(!std::empty((*img_res).get_data()), "(*img_res).get_data() is not valid");

        auto buf_inf = buf_info_t{};
        buf_inf.set_memory(buffer_memory_t::CPU_TO_GPU);
        buf_inf.set_size(std::size((*img_res).get_data()));

        return buf_inf;
    }

    auto tex_2d_loader_t::copy_image_to_buffer(std::shared_ptr<img_png_t> const& img_res, std::shared_ptr<buf_t> const& buf_res)->void
    {
        assert_t::check_bool(img_res, "(img_res) is not valid");
        assert_t::check_bool(buf_res, "(buf_res) is not valid");
        assert_t::check_bool(!std::empty((*img_res).get_data()), "(*img_res).get_data() is not valid");
        assert_t::check_bool(std::size((*img_res).get_data()) == (*buf_res).get_info().get_size(), "(img_res) and (buf_res) are not the same size");

        (*buf_res).set_data((*img_res).get_data());
    }

    auto tex_2d_loader_t::copy_buffer_to_texture(engine_t& engine, std::shared_ptr<buf_t> const& buf_res, std::shared_ptr<tex_2d_t> const& tex_res)->void
    {
        assert_t::check_bool(buf_res, "(buf_res) is not valid");
        assert_t::check_bool(tex_res, "(tex_res) is not valid");

        auto const cpy_cmd = std::make_shared<gpu_cmd_copy_buf_to_tex_2d_t>(engine);
        assert_t::check_bool(cpy_cmd, "(cpy_cmd) is not valid");

        (*cpy_cmd).set_src(buf_res);
        (*cpy_cmd).set_dst(tex_res);

        engine.get_graphic_executor().execute_command(cpy_cmd);
    }
}
