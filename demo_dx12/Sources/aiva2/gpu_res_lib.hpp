#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    struct gpu_res_lib_t final
    {
        // ------------------------------------------------

    private:
        gpu_res_lib_t() = default;

        // ------------------------------------------------

    private:
        static auto get_instance() -> gpu_res_lib_t&;

        // ------------------------------------------------

    public:
        template <typename t_res>
        static void reg_res();

        // ------------------------------------------------

    private:
        using copy_deep_key_type = std::type_index;

        using copy_deep_val_type = std::function<std::shared_ptr<gpu_res_t>(std::shared_ptr<gpu_res_t> const&)>;

        using copy_deep_hsh_type = boost::hash<copy_deep_key_type>;

    public:
        static auto copy_deep(std::shared_ptr<gpu_res_t> const& res) -> std::shared_ptr<gpu_res_t>;

    private:
        template <typename t_res>
        static void reg_copy_deep_func();

    private:
        std::unordered_map<copy_deep_key_type, copy_deep_val_type, copy_deep_hsh_type> m_copy_deep_funcs{};

        // ------------------------------------------------

    private:
        using copy_shlw_key_type = std::type_index;

        using copy_shlw_val_type = std::function<std::shared_ptr<gpu_res_t>(std::shared_ptr<gpu_res_t> const&)>;

        using copy_shlw_hsh_type = boost::hash<copy_shlw_key_type>;

    public:
        static auto copy_shlw(std::shared_ptr<gpu_res_t> const& res) -> std::shared_ptr<gpu_res_t>;

    private:
        template <typename t_res>
        static void reg_copy_shlw_func();

    private:
        std::unordered_map<copy_shlw_key_type, copy_shlw_val_type, copy_shlw_hsh_type> m_copy_shlw_funcs{};

        // ------------------------------------------------
    };
}

// --------------------------------------------------------

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/gpu_cmd_copy_resource.hpp>
#include <aiva2/graphic_executor.hpp>

namespace aiva2
{
    template <typename t_res>
    void gpu_res_lib_t::reg_res()
    {
        reg_copy_deep_func<t_res>();
        reg_copy_shlw_func<t_res>();
    }

    template <typename t_res>
    void gpu_res_lib_t::reg_copy_deep_func()
    {
        auto const key = copy_deep_key_type{ typeid(t_res) };
        auto const val = [](std::shared_ptr<gpu_res_t> const& res_raw) -> std::shared_ptr<gpu_res_t>
        {
            auto const res_src = std::dynamic_pointer_cast<t_res>(res_raw);
            assert_t::check_bool(res_src, "(res_src) is not valid");

            auto const res_dst = std::make_shared<t_res>((*res_src).get_engine(), (*res_src).get_info());
            assert_t::check_bool(res_dst, "(res_dst) is not valid");

            auto const cmd_cpy = std::make_shared<gpu_cmd_copy_resource_t>((*res_src).get_engine());
            assert_t::check_bool(cmd_cpy, "(cmd_cpy) is not valid");

            (*cmd_cpy).set_src(res_src);
            (*cmd_cpy).set_dst(res_dst);

            (*res_src).get_engine().get_graphic_executor().execute_command(cmd_cpy);

            return res_dst;
        };
        
        get_instance().m_copy_deep_funcs[key] = val;
    }

    template <typename t_res>
    void gpu_res_lib_t::reg_copy_shlw_func()
    {
        auto const key = copy_shlw_key_type{ typeid(t_res) };
        auto const val = [](std::shared_ptr<gpu_res_t> const& res_raw) -> std::shared_ptr<gpu_res_t>
        {
            auto const res_src = std::dynamic_pointer_cast<t_res>(res_raw);
            assert_t::check_bool(res_src, "(res_src) is not valid");

            return std::make_shared<t_res>((*res_src).get_engine(), (*res_src).get_resource());
        };

        get_instance().m_copy_shlw_funcs[key] = val;
    }
}