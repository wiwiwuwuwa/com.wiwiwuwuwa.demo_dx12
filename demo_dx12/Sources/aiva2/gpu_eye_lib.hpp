#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_eye_key.hpp>
#include <aiva2/gpu_eye_key_utils.hpp>

namespace aiva2
{
    struct gpu_eye_lib_t final
    {
        // ------------------------------------------------

    private:
        gpu_eye_lib_t() = default;

        // ------------------------------------------------

    private:
        static auto get_instance() -> gpu_eye_lib_t&;

        // ------------------------------------------------

    public:
        template <typename t_eye>
        static void reg_eye();

        // ------------------------------------------------

    private:
        using get_res_key_type = std::type_index;

        using get_res_val_type = std::function<std::shared_ptr<gpu_res_t>(std::shared_ptr<gpu_eye_t> const&)>;

        using get_res_hsh_type = boost::hash<get_res_key_type>;

    public:
        static auto get_res(std::shared_ptr<gpu_eye_t> const& eye) -> std::shared_ptr<gpu_res_t>;

    private:
        template <typename t_eye>
        static void reg_get_res_func();

    private:
        std::unordered_map<get_res_key_type, get_res_val_type, get_res_hsh_type> m_get_res_funcs{};

        // ------------------------------------------------

    private:
        using get_inf_key_type = std::type_index;

        using get_inf_val_type = std::function<std::shared_ptr<gpu_eye_info_t>(std::shared_ptr<gpu_eye_t> const&)>;

        using get_inf_hsh_type = boost::hash<get_inf_key_type>;

    public:
        static auto get_inf(std::shared_ptr<gpu_eye_t> const& eye) -> std::shared_ptr<gpu_eye_info_t>;

    private:
        template <typename t_eye>
        static void reg_get_inf_func();

    private:
        std::unordered_map<get_inf_key_type, get_inf_val_type, get_inf_hsh_type> m_get_inf_funcs{};

        // ------------------------------------------------

    private:
        using new_eye_key_type = std::pair<std::type_index, std::type_index>;

        using new_eye_val_type = std::function<std::shared_ptr<gpu_eye_t>(engine_t&, std::shared_ptr<gpu_res_t> const&, std::shared_ptr<gpu_eye_info_t> const&)>;

        using new_eye_hsh_type = boost::hash<new_eye_key_type>;

    public:
        static auto new_eye(engine_t& engine, std::shared_ptr<gpu_res_t> const& resource, std::shared_ptr<gpu_eye_info_t> const& info)->std::shared_ptr<gpu_eye_t>;

    private:
        template <typename t_eye>
        static void reg_new_eye_func();
        
    private:
        std::unordered_map<new_eye_key_type, new_eye_val_type, new_eye_hsh_type> m_new_eye_funcs{};

        // ------------------------------------------------

    private:
        using copy_deep_key_type = std::type_index;

        using copy_deep_val_type = std::function<std::shared_ptr<gpu_eye_t>(std::shared_ptr<gpu_eye_t> const&)>;

        using copy_deep_hsh_type = boost::hash<copy_deep_key_type>;

    public:
        static auto copy_deep(std::shared_ptr<gpu_eye_t> const& eye) -> std::shared_ptr<gpu_eye_t>;

    private:
        template <typename t_eye>
        static void reg_copy_deep_func();

    private:
        std::unordered_map<copy_deep_key_type, copy_deep_val_type, copy_deep_hsh_type> m_copy_deep_funcs{};

        // ------------------------------------------------

    private:
        using copy_shlw_key_type = std::type_index;

        using copy_shlw_val_type = std::function<std::shared_ptr<gpu_eye_t>(std::shared_ptr<gpu_eye_t> const&)>;

        using copy_shlw_hsh_type = boost::hash<copy_shlw_key_type>;

    public:
        static auto copy_shlw(std::shared_ptr<gpu_eye_t> const& eye) -> std::shared_ptr<gpu_eye_t>;

    private:
        template <typename t_eye>
        static void reg_copy_shlw_func();

    private:
        std::unordered_map<copy_shlw_key_type, copy_shlw_val_type, copy_shlw_hsh_type> m_copy_shlw_funcs{};

        // ------------------------------------------------

    private:
        using get_viewport_size_key_type = std::type_index;

        using get_viewport_size_val_type = std::function<glm::size2(std::shared_ptr<gpu_eye_t> const&)>;

        using get_viewport_size_hsh_type = boost::hash<get_viewport_size_key_type>;

    public:
        static auto get_viewport_size(std::shared_ptr<gpu_eye_t> const& eye) -> glm::size2;

    private:
        template <typename t_eye>
        static void reg_get_viewport_size_func();

    private:
        std::unordered_map<get_viewport_size_key_type, get_viewport_size_val_type, get_viewport_size_hsh_type> m_get_viewport_size_funcs{};

        // ------------------------------------------------
    };
};

// --------------------------------------------------------

#include <aiva2/gpu_res_lib.hpp>

namespace aiva2
{
    template <typename t_eye>
    void gpu_eye_lib_t::reg_eye()
    {
        reg_get_res_func<t_eye>();
        reg_get_inf_func<t_eye>();
        reg_new_eye_func<t_eye>();
        reg_copy_deep_func<t_eye>();
        reg_copy_shlw_func<t_eye>();
        reg_get_viewport_size_func<t_eye>();
    }
    
    template <typename t_eye>
    void gpu_eye_lib_t::reg_get_res_func()
    {
        auto const key = get_res_key_type{ typeid(t_eye) };
        auto const val = [](std::shared_ptr<gpu_eye_t> const& eye) -> std::shared_ptr<gpu_res_t>
        {
            auto const typed_eye = std::dynamic_pointer_cast<t_eye>(eye);
            assert_t::check_bool(typed_eye, "typed_eye is not valid");

            return (*typed_eye).get_resource_ptr();
        };
        
        get_instance().m_get_res_funcs[key] = val;
    }

    template <typename t_eye>
    void gpu_eye_lib_t::reg_get_inf_func()
    {
        auto const key = get_inf_key_type{ typeid(t_eye) };
        auto const val = [](std::shared_ptr<gpu_eye_t> const& eye) -> std::shared_ptr<gpu_eye_info_t>
        {
            auto const typed_eye = std::dynamic_pointer_cast<t_eye>(eye);
            assert_t::check_bool(typed_eye, "typed_eye is not valid");

            return std::make_shared<typename t_eye::info_type>((*typed_eye).get_info());
        };

        get_instance().m_get_inf_funcs[key] = val;
    }

    template <typename t_eye>
    void gpu_eye_lib_t::reg_new_eye_func()
    {
        auto const key = new_eye_key_type{ typeid(typename t_eye::resource_type), typeid(typename t_eye::info_type) };
        auto const val = [](engine_t& engine, std::shared_ptr<gpu_res_t> const& resource, std::shared_ptr<gpu_eye_info_t> const& info) -> std::shared_ptr<gpu_eye_t>
        {
            auto const typed_resource = std::dynamic_pointer_cast<typename t_eye::resource_type>(resource);
            auto const typed_info = std::dynamic_pointer_cast<typename t_eye::info_type>(info);

            assert_t::check_bool(typed_resource, "typed_resource is not valid");
            assert_t::check_bool(typed_info, "typed_info is not valid");
            
            return std::make_shared<t_eye>(engine, typed_resource, *typed_info);
        };
        
        get_instance().m_new_eye_funcs[key] = val;
    }

    template <typename t_eye>
    void gpu_eye_lib_t::reg_copy_deep_func()
    {
        auto const key = copy_deep_key_type{ typeid(t_eye) };
        auto const val = [](std::shared_ptr<gpu_eye_t> const& eye) -> std::shared_ptr<gpu_eye_t>
        {
            auto const typed_eye = std::dynamic_pointer_cast<t_eye>(eye);
            assert_t::check_bool(typed_eye, "(typed_eye) is not valid");

            auto const& src_res = (*typed_eye).get_resource_ptr();
            auto const& dst_res = std::dynamic_pointer_cast<t_eye::resource_type>(gpu_res_lib_t::copy_deep(src_res));
            assert_t::check_bool(dst_res, "(dst_res) is not valid");
            
            return std::make_shared<t_eye>((*typed_eye).get_engine(), dst_res, (*typed_eye).get_info());
        };
        
        get_instance().m_copy_deep_funcs[key] = val;
    }

    template <typename t_eye>
    void gpu_eye_lib_t::reg_copy_shlw_func()
    {
        auto const key = copy_shlw_key_type{ typeid(t_eye) };
        auto const val = [](std::shared_ptr<gpu_eye_t> const& eye) -> std::shared_ptr<gpu_eye_t>
        {
            auto const typed_eye = std::dynamic_pointer_cast<t_eye>(eye);
            assert_t::check_bool(typed_eye, "(typed_eye) is not valid");

            return std::make_shared<t_eye>((*typed_eye).get_engine(), (*typed_eye).get_resource_ptr(), (*typed_eye).get_info());
        };
        
        get_instance().m_copy_shlw_funcs[key] = val;
    }

    template <typename t_eye>
    void gpu_eye_lib_t::reg_get_viewport_size_func()
    {
        auto const key = get_viewport_size_key_type{ typeid(t_eye) };
        auto const val = [](std::shared_ptr<gpu_eye_t> const& eye) -> glm::size2
        {
            auto const typed_eye = std::dynamic_pointer_cast<t_eye>(eye);
            assert_t::check_bool(typed_eye, "(typed_eye) is not valid");

            auto const typed_res = (*typed_eye).get_resource_ptr();
            assert_t::check_bool(typed_res, "(typed_res) is not valid");

            return glm::size2
            (
                (*typed_res).get_info().get_width(),
                (*typed_res).get_info().get_height()
            )
            >> (*typed_eye).get_info().get_mip_slice();
        };
        
        get_instance().m_get_viewport_size_funcs[key] = val;
    }
}
