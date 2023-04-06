#pragma once
#include <aiva2/_core.hpp>
#include <aiva2/buf_info.hpp>
#include <aiva2/tex_2d_info.hpp>

namespace aiva2
{
    struct tex_2d_loader_t final
    {
        // ------------------------------------------------

    private:
        tex_2d_loader_t() = delete;

        // ------------------------------------------------

    public:
        static auto load(engine_t& engine, nlohmann::json const& json)->std::shared_ptr<tex_2d_t>;

    private:
        static auto load_image(engine_t& engine, nlohmann::json const& json)->std::shared_ptr<img_png_t>;

        static auto create_texture(engine_t& engine, nlohmann::json const& json, std::shared_ptr<img_png_t> const& img_res)->std::shared_ptr<tex_2d_t>;

        static auto create_texture_info(nlohmann::json const& json, std::shared_ptr<img_png_t> const& img_res)->tex_2d_info_t;

        static auto copy_image_to_texture(engine_t& engine, std::shared_ptr<img_png_t> const& img_res, std::shared_ptr<tex_2d_t> const& tex_res)->void;

        static auto create_buffer(engine_t& engine, std::shared_ptr<img_png_t> const& img_res)->std::shared_ptr<buf_t>;

        static auto create_buffer_info(std::shared_ptr<img_png_t> const& img_res)->buf_info_t;

        static auto copy_image_to_buffer(std::shared_ptr<img_png_t> const& img_res, std::shared_ptr<buf_t> const& buf_res)->void;

        static auto copy_buffer_to_texture(engine_t& engine, std::shared_ptr<buf_t> const& buf_res, std::shared_ptr<tex_2d_t> const& tex_res)->void;

        // ------------------------------------------------
    };
}
