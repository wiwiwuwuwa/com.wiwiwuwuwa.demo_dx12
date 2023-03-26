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

    private:
        using eye_key_type = std::pair<std::type_index, std::type_index>;

        using eye_val_type = std::function<std::shared_ptr<gpu_eye_t>(engine_t&, std::shared_ptr<gpu_res_t> const&, std::shared_ptr<gpu_eye_info_t> const&)>;

        using eye_hsh_type = boost::hash<eye_key_type>;

    public:
        template <typename t_eye>
        static void reg_eye();

    public:
        static auto new_eye(engine_t& engine, std::shared_ptr<gpu_res_t> const& resource, std::shared_ptr<gpu_eye_info_t> const& info)->std::shared_ptr<gpu_eye_t>;
        
    private:
        std::unordered_map<eye_key_type, eye_val_type, eye_hsh_type> m_eyes{};

        // ------------------------------------------------
    };
};

// --------------------------------------------------------

namespace aiva2
{
    template <typename t_eye>
    void gpu_eye_lib_t::reg_eye()
    {
        auto const key = eye_key_type{ typeid(typename t_eye::resource_type), typeid(typename t_eye::info_type) };
        auto const val = [](engine_t& engine, std::shared_ptr<gpu_res_t> const& resource, std::shared_ptr<gpu_eye_info_t> const& info) -> std::shared_ptr<gpu_eye_t>
        {
            auto const typed_resource = std::dynamic_pointer_cast<typename t_eye::resource_type>(resource);
            auto const typed_info = std::dynamic_pointer_cast<typename t_eye::info_type>(info);

            assert_t::check_bool(typed_resource, "typed_resource is not valid");
            assert_t::check_bool(typed_info, "typed_info is not valid");
            
            return std::make_shared<t_eye>(engine, typed_resource, *typed_info);
        };
        
        get_instance().m_eyes[key] = val;
    }
}
