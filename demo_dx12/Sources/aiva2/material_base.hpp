#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>
#include <aiva2/shader_info_for_resource_utils.hpp>

namespace aiva2
{
    struct material_base_t : public implements_t<material_base_t, engine_object_t>
    {
        // ------------------------------------------------

    protected:
        material_base_t(engine_t& engine);

        ~material_base_t() override;

    public:
        auto init_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

        auto shut_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

        // ------------------------------------------------

    public:
        auto get_shader_ref() const->shader_base_t const&;

        auto get_shader_ptr() const->std::shared_ptr<shader_base_t const>;

    protected:
        virtual auto get_shader_imp() const->std::shared_ptr<shader_base_t const> = 0;

        // ------------------------------------------------

    private:
        using res_key_type = std::shared_ptr<shader_info_for_resource_t const>;

        using res_val_type = std::shared_ptr<gpu_eye_t>;

    public:
        void set_resource(std::string const& name, std::shared_ptr<gpu_res_t> const& resource, std::shared_ptr<gpu_eye_info_t> const& info);

    public:
        auto get_resource_heap_ref() const->ID3D12DescriptorHeap&;

        auto get_resource_heap_ptr() const->winrt::com_ptr<ID3D12DescriptorHeap> const&;

    private:
        void init_resources();

        void tick_resources();

        void shut_resources();

    private:
        auto init_resources_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

        auto shut_resources_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

    private:
        std::unordered_map<std::string, res_key_type> m_resources_keys{};

        std::map<res_key_type, res_val_type> m_resources_data{};

        winrt::com_ptr<ID3D12DescriptorHeap> m_resources_heap{};

        // ------------------------------------------------
    };
}
