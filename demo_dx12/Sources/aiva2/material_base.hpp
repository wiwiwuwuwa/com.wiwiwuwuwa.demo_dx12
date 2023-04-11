#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>

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
        auto get_resource(std::string const& name) const->std::shared_ptr<gpu_res_t>;

        auto get_resource(size_t const index) const->std::shared_ptr<gpu_res_t>;

        auto get_info(std::string const& name) const->std::shared_ptr<gpu_eye_info_t>;

        auto get_info(size_t const index) const->std::shared_ptr<gpu_eye_info_t>;

        auto get_name(size_t const index) const->std::string;

        auto get_name(std::shared_ptr<gpu_res_t> const& resource) const->std::string;

        auto get_index(std::string const& name) const->std::optional<size_t>;

        auto get_index(std::shared_ptr<gpu_res_t> const& resource) const->std::optional<size_t>;

        auto num_resources() const->size_t;

        void set_resource(std::string const& name, std::shared_ptr<gpu_res_t> const& resource, std::shared_ptr<gpu_eye_info_t> const& info);

    public:
        auto get_resource_heap_ref() const->ID3D12DescriptorHeap&;

        auto get_resource_heap_ptr() const->winrt::com_ptr<ID3D12DescriptorHeap> const&;

    protected:
        void init_resources(shader_info_for_code_t const& code_info);

        void shut_resources();

    private:
        void tick_resources();

    private:
        auto init_resources_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

        auto shut_resources_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

    private:
        std::unordered_map<std::string, size_t> m_resources_keys{};

        std::vector<std::shared_ptr<gpu_eye_t>> m_resources_data{};

        winrt::com_ptr<ID3D12DescriptorHeap> m_resources_heap{};

        // ------------------------------------------------
    };
}
