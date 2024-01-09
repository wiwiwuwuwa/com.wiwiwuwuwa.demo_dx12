#pragma once
#include <aiva/graphic_format.hpp>
#include <aiva/i_support_unordered_access.hpp>
#include <aiva/object.hpp>

namespace aiva
{
    struct texture_2d_info_t : public implements_t<texture_2d_info_t, object_t, i_support_unordered_access_t>
    {
        // ------------------------------------------------

    public:
        auto get_format() const -> graphic_format_t;

        void set_format(graphic_format_t const format);

    private:
        graphic_format_t m_format{};

        // ------------------------------------------------

    public:
        auto get_width() const -> size_t;

        void set_width(size_t const width);

    private:
        size_t m_width{};

        // ------------------------------------------------

    public:
        auto get_height() const -> size_t;

        void set_height(size_t const height);

    private:
        size_t m_height{};

        // ------------------------------------------------

    public:
        auto get_support_mip_mappping() const -> bool;

        void set_support_mip_mappping(bool const support_mip_mappping);

    private:
        bool m_support_mip_mappping{};

        // ------------------------------------------------

    public:
        auto get_support_shader_resource() const -> bool;

        void set_support_shader_resource(bool const support_shader_resource);

    private:
        bool m_support_shader_resource{};

        // ------------------------------------------------

    public:
        auto get_support_shader_atomics() const -> bool;

        void set_support_shader_atomics(bool const support_shader_atomics);

    private:
        bool m_support_shader_atomics{};

        // ------------------------------------------------

    public:
        auto get_support_render_target() const -> bool;

        void set_support_render_target(bool const support_render_target);

    private:
        bool m_support_render_target{};

        // ------------------------------------------------

    public:
        auto get_support_depth_stencil() const -> bool;

        void set_support_depth_stencil(bool const support_depth_stencil);

    private:
        bool m_support_depth_stencil{};

        // ------------------------------------------------

    public:
        auto get_support_unordered_access() const -> bool;

        void set_support_unordered_access(bool const support_unordered_access);

    private:
        bool m_support_unordered_access{};

        // ------------------------------------------------
    };
} // namespace aiva
