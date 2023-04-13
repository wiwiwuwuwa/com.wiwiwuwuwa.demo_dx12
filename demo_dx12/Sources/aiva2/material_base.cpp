#include <pch.h>
#include <aiva2/material_base.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/d3d12_cpu_descriptor_handle_utils.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/gpu_eye.hpp>
#include <aiva2/gpu_eye_info.hpp>
#include <aiva2/gpu_eye_lib.hpp>
#include <aiva2/gpu_res.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/shader_info_for_code.hpp>
#include <aiva2/shader_info_for_resource.hpp>

namespace aiva2
{
    material_base_t::material_base_t(engine_t& engine)
        : impl_type{ engine }
    {
        
    }

    material_base_t::~material_base_t()
    {
        
    }

    auto material_base_t::init_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
    {
        return init_resources_for_rendering();
    }

    auto material_base_t::shut_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
    {
        return shut_resources_for_rendering();
    }

    auto material_base_t::get_resource(std::string const& name) const->std::shared_ptr<gpu_res_t>
    {
        auto const idx = get_index(name);
        if (!idx) return {};

        return get_resource(*idx);
    }

    auto material_base_t::get_resource(size_t const index) const->std::shared_ptr<gpu_res_t>
    {
        auto const res = get_view(index);
        if (!res) return {};

        return gpu_eye_lib_t::get_res(res);
    }

    auto material_base_t::get_info(std::string const& name) const->std::shared_ptr<gpu_eye_info_t>
    {
        auto const idx = get_index(name);
        if (!idx) return {};

        return get_info(*idx);
    }

    auto material_base_t::get_info(size_t const index) const->std::shared_ptr<gpu_eye_info_t>
    {
        auto const res = get_view(index);
        if (!res) return {};

        return gpu_eye_lib_t::get_inf(res);
    }

    auto material_base_t::get_view(std::string const& name) const->std::shared_ptr<gpu_eye_t>
    {
        auto const idx = get_index(name);
        if (!idx) return {};

        return get_view(*idx);
    }

    auto material_base_t::get_view(size_t const index) const->std::shared_ptr<gpu_eye_t>
    {
        if (index < size_t{}) return {};
        if (index >= std::size(m_resources_data)) return {};

        return m_resources_data[index];
    }

    auto material_base_t::get_name(size_t const index) const->std::string
    {
        auto const itr = std::find_if(std::cbegin(m_resources_keys), std::cend(m_resources_keys), [index](auto const& pair) { return pair.second == index; });
        return itr != std::cend(m_resources_keys) ? (*itr).first : std::string{};
    }

    auto material_base_t::get_name(std::shared_ptr<gpu_res_t> const& resource) const->std::string
    {
        auto const idx = get_index(resource);
        if (!idx) return {};

        return get_name(*idx);
    }

    auto material_base_t::get_name(std::shared_ptr<gpu_eye_t> const& view) const->std::string
    {
        auto const idx = get_index(view);
        if (!idx) return {};

        return get_name(*idx);
    }

    auto material_base_t::get_index(std::string const& name) const->std::optional<size_t>
    {
        if (std::empty(name)) return {};

        auto const res_key = m_resources_keys.find(name);
        if (res_key == std::cend(m_resources_keys)) return {};

        auto const res_idx = (*res_key).second;
        if (res_idx < size_t{}) return {};
        if (res_idx >= std::size(m_resources_data)) return {};

        return res_idx;
    }

    auto material_base_t::get_index(std::shared_ptr<gpu_res_t> const& resource) const->std::optional<size_t>
    {
        if (!resource) return {};

        auto const res_itr = std::find_if(std::cbegin(m_resources_data), std::cend(m_resources_data), [resource](auto const& res) { return gpu_eye_lib_t::get_res(res) == resource; });
        if (res_itr == std::cend(m_resources_data)) return {};

        auto const res_idx = static_cast<size_t>(std::distance(std::cbegin(m_resources_data), res_itr));
        if (res_idx < size_t{}) return {};
        if (res_idx >= std::size(m_resources_data)) return {};

        return res_idx;
    }

    auto material_base_t::get_index(std::shared_ptr<gpu_eye_t> const& view) const->std::optional<size_t>
    {
        if (!view) return {};

        auto const res_itr = std::find(std::cbegin(m_resources_data), std::cend(m_resources_data), view);
        if (res_itr == std::cend(m_resources_data)) return {};

        auto const res_idx = static_cast<size_t>(std::distance(std::cbegin(m_resources_data), res_itr));
        if (res_idx < size_t{}) return {};
        if (res_idx >= std::size(m_resources_data)) return {};

        return res_idx;
    }

    auto material_base_t::num_resource() const->size_t
    {
        return std::size(m_resources_data);
    }

    void material_base_t::set_resource(std::string const& name, std::shared_ptr<gpu_res_t> const& resource, std::shared_ptr<gpu_eye_info_t> const& info)
    {
        assert_t::check_bool(!std::empty(name), "(name) not valid");
        assert_t::check_bool(resource, "(resource) not valid");
        assert_t::check_bool(info, "(info) not valid");

        auto const res_val = gpu_eye_lib_t::new_eye(get_engine(), resource, info);
        assert_t::check_bool(res_val, "res_val is not valid");

        set_resource(name, res_val);
    }

    void material_base_t::set_resource(size_t const index, std::shared_ptr<gpu_res_t> const& resource, std::shared_ptr<gpu_eye_info_t> const& info)
    {
        assert_t::check_bool(index >= size_t{}, "(index) is not valid");
        assert_t::check_bool(index < std::size(m_resources_data), "(index) is not valid");
        assert_t::check_bool(resource, "(resource) not valid");
        assert_t::check_bool(info, "(info) not valid");

        auto const res_val = gpu_eye_lib_t::new_eye(get_engine(), resource, info);
        assert_t::check_bool(res_val, "(res_val) is not valid");

        set_resource(index, res_val);
    }
        
    void material_base_t::set_resource(std::string const& name, std::shared_ptr<gpu_eye_t> const& view)
    {
        assert_t::check_bool(!std::empty(name), "(name) not valid");
        assert_t::check_bool(view, "(view) not valid");

        auto const res_idx = get_index(name);
        assert_t::check_bool(res_idx, "(res_idx) not valid");
        
        set_resource(*res_idx, view);
    }

    void material_base_t::set_resource(size_t const index, std::shared_ptr<gpu_eye_t> const& view)
    {
        assert_t::check_bool(index >= size_t{}, "index is not valid");
        assert_t::check_bool(index < std::size(m_resources_data), "index is not valid");
        assert_t::check_bool(view, "view not valid");

        m_resources_data[index] = view;
        tick_resources();
    }
    
    auto material_base_t::get_resource_heap_ref() const->ID3D12DescriptorHeap&
    {
        assert_t::check_bool(m_resources_heap, "m_resources_heap is not valid");
        return (*m_resources_heap);
    }

    auto material_base_t::get_resource_heap_ptr() const->winrt::com_ptr<ID3D12DescriptorHeap> const&
    {
        return m_resources_heap;
    }

    void material_base_t::init_resources(shader_info_for_code_t const& code_info)
    {
        m_resources_keys = {};
        m_resources_data = {};

        for (auto i = size_t{}; i < code_info.num_resource(); i++)
        {
            auto const& resource_data = code_info.get_resource_ptr(i);
            assert_t::check_bool(resource_data, "resource_data is not valid");
            
            if ((*resource_data).get_frequency() != shader_resource_frequency_t::PER_MATERIAL) continue;

            auto const& resource_name = (*resource_data).get_name();
            assert_t::check_bool(!std::empty(resource_name), "resource_name is not valid");

            m_resources_keys.insert_or_assign(resource_name, std::size(m_resources_data));
            m_resources_data.push_back(nullptr);
        }
        
        m_resources_heap = {};

        if (std::empty(m_resources_data))
        {
            return;
        }

        auto heap_desc = D3D12_DESCRIPTOR_HEAP_DESC{};
        heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        heap_desc.NumDescriptors = static_cast<UINT>(std::size(m_resources_data));
        heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        heap_desc.NodeMask = {};

        assert_t::check_hresult(get_engine().get_graphic_hardware().get_device().CreateDescriptorHeap
        (
            /*pDescriptorHeapDesc*/ &heap_desc,
            /*Heap*/ IID_PPV_ARGS(&m_resources_heap)
        ));
        assert_t::check_bool(m_resources_heap, "m_resources_heap is not valid");
    }

    void material_base_t::shut_resources()
    {
        m_resources_heap = {};
        m_resources_data = {};
        m_resources_keys = {};
    }

    void material_base_t::tick_resources()
    {
        auto const increment_size = get_engine().get_graphic_hardware().get_device().GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        assert_t::check_bool(increment_size > UINT{}, "increment_size is not valid");

        for (auto i = size_t{}; i < std::size(m_resources_data); ++i)
        {
            auto const& resource_data = m_resources_data[i];
            if (!resource_data) continue;

            auto const bind_place = (*m_resources_heap).GetCPUDescriptorHandleForHeapStart() + i * increment_size;
            (*resource_data).bind_for_rendering(bind_place);
        }
    }

    auto material_base_t::init_resources_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
    {
        auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

        for (auto const& resource_data : m_resources_data)
        {
            if (!resource_data) continue;

            auto const resource_barriers = (*resource_data).init_for_rendering();
            std::move(std::cbegin(resource_barriers), std::cend(resource_barriers), std::back_inserter(barriers));
        }

        return barriers;
    }

    auto material_base_t::shut_resources_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
    {
        auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

        for (auto const& resource_data : m_resources_data)
        {
            if (!resource_data) continue;

            auto const resource_barriers = (*resource_data).shut_for_rendering();
            std::move(std::cbegin(resource_barriers), std::cend(resource_barriers), std::back_inserter(barriers));
        }

        return barriers;
    }
}
