#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    struct mat_lib_t final
    {
        // ------------------------------------------------

    private:
        mat_lib_t() = default;

        // ------------------------------------------------

    private:
        static auto get_instance() -> mat_lib_t&;

        // ------------------------------------------------

    public:
        template <typename t_mat>
        static void reg_mat();

        // ------------------------------------------------

    private:
        using copy_key_type = std::type_index;

        using copy_val_type = std::function<std::shared_ptr<material_base_t>(std::shared_ptr<material_base_t> const&)>;

        using copy_hsh_type = boost::hash<copy_key_type>;

    public:
        static auto copy(std::shared_ptr<material_base_t> const& mat) -> std::shared_ptr<material_base_t>;

    private:
        template <typename t_mat>
        static void reg_copy_func();

    private:
        std::unordered_map<copy_key_type, copy_val_type, copy_hsh_type> m_copy_funcs{};

        // ------------------------------------------------
    };
}

// --------------------------------------------------------

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/gpu_eye.hpp>
#include <aiva2/gpu_eye_lib.hpp>

namespace aiva2
{
    template <typename t_mat>
    void mat_lib_t::reg_mat()
    {
        reg_copy_func<t_mat>();
    }

    template <typename t_mat>
    void mat_lib_t::reg_copy_func()
    {
        auto const key = copy_key_type{ typeid(t_mat) };
        auto const val = [](std::shared_ptr<material_base_t> const& mat_raw) -> std::shared_ptr<material_base_t>
        {
            auto const mat_src = std::dynamic_pointer_cast<t_mat>(mat_raw);
            assert_t::check_bool(mat_src, "(mat_src) is not valid");
            
            auto const mat_dst = std::make_shared<t_mat>((*mat_src).get_engine(), (*mat_src).get_shader_ptr());
            assert_t::check_bool(mat_dst, "(mat_dst) is not valid");

            for (auto i = size_t{}; i < (*mat_src).num_resource(); i++)
            {
                auto const res_src = (*mat_src).get_view(i);
                if (!res_src) continue;

                auto const nme_src = (*mat_src).get_name(i);
                assert_t::check_bool(!nme_src.empty(), "(nme_src) is not valid");

                auto res_dst = std::shared_ptr<gpu_eye_t>{};

                if (std::regex_search(nme_src, std::regex{ R"(per_object)" }))
                    res_dst = gpu_eye_lib_t::copy_deep(res_src);
                else
                    res_dst = gpu_eye_lib_t::copy_shlw(res_src);

                assert_t::check_bool(res_dst, "(res_dst) is not valid");
                (*mat_dst).set_resource(i, res_dst);                    
            }

            return mat_dst;
        };

        get_instance().m_copy_funcs[key] = val;
    }
}
