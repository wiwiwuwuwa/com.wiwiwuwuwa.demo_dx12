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
#include <aiva2/shader_base.hpp>
#include <aiva2/shader_info.hpp>
#include <aiva2/shader_info_for_code.hpp>
#include <aiva2/shader_info_for_resource.hpp>

namespace aiva2
{
    material_base_t::material_base_t(engine_t& engine)
        : impl_type{ engine }
    {
        init_resources();
    }

    material_base_t::~material_base_t()
    {
        shut_resources();
    }

    auto material_base_t::init_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
    {
        return init_resources_for_rendering();
    }

    auto material_base_t::shut_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
    {
        return shut_resources_for_rendering();
    }
    
    auto material_base_t::get_shader_ref() const->shader_base_t const&
    {
        auto const shader = get_shader_imp();
        assert_t::check_bool(shader, "shader is not valid");

        return (*shader);
    }

    auto material_base_t::get_shader_ptr() const->std::shared_ptr<shader_base_t const>
    {
        return get_shader_imp();
    }

    void material_base_t::set_resource(std::string const& name, std::shared_ptr<gpu_res_t> const& resource, std::shared_ptr<gpu_eye_info_t> const& info)
    {
        assert_t::check_bool(!std::empty(name), "name not valid");
        assert_t::check_bool(resource, "resource not valid");
        assert_t::check_bool(info, "info not valid");

        auto const res_key = m_resources_keys.find(name);
        assert_t::check_bool(res_key != std::cend(m_resources_keys), "res_key is not valid");
        assert_t::check_bool((*res_key).second >= size_t{}, "(*res_key).second is not valid");
        assert_t::check_bool((*res_key).second < std::size(m_resources_data), "(*res_key).second is not valid");

        auto const res_val = gpu_eye_lib_t::new_eye(get_engine(), resource, info);
        assert_t::check_bool(res_val, "res_val is not valid");

        m_resources_data[(*res_key).second] = res_val;
        tick_resources();
    }
    
    auto material_base_t::get_resource_heap_ref() const->ID3D12DescriptorHeap&
    {
        assert_t::check_bool(m_resources_heap, "m_resources_heap is not valid");
        return (*m_resources_heap);
    }

    auto material_base_t::get_resource_heap_ptr() const->winrt::com_ptr<ID3D12DescriptorHeap> const&
    {
        assert_t::check_bool(m_resources_heap, "m_resources_heap is not valid");
        return m_resources_heap;
    }

    void material_base_t::init_resources()
    {
        auto const& code_block = get_shader_ref().get_info().get_code_block();

        m_resources_keys = {};
        m_resources_data = {};

        for (auto i = size_t{}; i < code_block.num_resource(); i++)
        {
            auto const& resource_data = code_block.get_resource_ptr(i);
            assert_t::check_bool(resource_data, "resource_data is not valid");

            auto const& resource_name = resource_data->get_name();
            assert_t::check_bool(!std::empty(resource_name), "resource_name is not valid");

            m_resources_keys.insert_or_assign(resource_name, i);
            m_resources_data.push_back(nullptr);
        }

        m_resources_heap = {};

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

    void material_base_t::shut_resources()
    {
        m_resources_heap = {};
        m_resources_data = {};
        m_resources_keys = {};
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
