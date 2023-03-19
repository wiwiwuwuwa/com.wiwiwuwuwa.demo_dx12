#include <pch.h>
#include <aiva2/d3d12_root_signature_desc.hpp>

#include <aiva2/shader_info.hpp>
#include <aiva2/shader_info_for_code.hpp>
#include <aiva2/shader_info_for_meta.hpp>
#include <aiva2/shader_info_for_resource.hpp>
#include <aiva2/shader_register_type_utils.hpp>

namespace aiva2
{
	d3d12_root_signature_desc_t::d3d12_root_signature_desc_t(shader_info_t const& shader_info)
	{
		init_descriptor_ranges(shader_info);
		init_root_parameters(shader_info);
		init_root_signature_desc(shader_info);
	}

	d3d12_root_signature_desc_t::~d3d12_root_signature_desc_t()
	{
		shut_root_signature_desc();
		shut_root_parameters();
		shut_descriptor_ranges();
	}

	void d3d12_root_signature_desc_t::init_descriptor_ranges(shader_info_t const& shader_info)
	{
		m_descriptor_ranges = {};

		for (auto i = size_t{}; i < shader_info.get_code_block().num_resource(); i++)
		{
			auto const& resource = shader_info.get_code_block().get_resource(i);

			auto const register_type = resource.get_register_type();
			assert_t::check_bool(is_valid(register_type), "register_type is not valid");

			auto const register_group_type = to_group_type(register_type);
			assert_t::check_bool(is_valid(register_group_type), "register_group_type is not valid");

			auto& descriptor_range = m_descriptor_ranges[register_group_type].emplace_back();
			descriptor_range.RangeType = to_d3d12_descriptor_range_type(register_type);
			descriptor_range.NumDescriptors = 1;
			descriptor_range.BaseShaderRegister = static_cast<UINT>(resource.get_register_index());
			descriptor_range.RegisterSpace = static_cast<UINT>(resource.get_register_space());
			descriptor_range.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
			descriptor_range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		}
	}
	
	void d3d12_root_signature_desc_t::shut_descriptor_ranges()
	{
		m_descriptor_ranges = {};
	}
	
	void d3d12_root_signature_desc_t::init_root_parameters(shader_info_t const&)
	{
		m_root_parameters = {};

		for (auto const& [register_group_type, descriptor_ranges] : m_descriptor_ranges)
		{
			auto& root_parameter = m_root_parameters.emplace_back();
			root_parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			root_parameter.DescriptorTable.NumDescriptorRanges = static_cast<UINT>(std::size(descriptor_ranges));
			root_parameter.DescriptorTable.pDescriptorRanges = std::data(descriptor_ranges);
			root_parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		}
	}

	void d3d12_root_signature_desc_t::shut_root_parameters()
	{
		m_root_parameters = {};
	}

	auto d3d12_root_signature_desc_t::get_root_signature_desc() const -> D3D12_VERSIONED_ROOT_SIGNATURE_DESC const&
	{
		assert_t::check_bool(m_root_signature_desc, "m_root_signature_desc is not valid");
		return (*m_root_signature_desc);
	}

	void d3d12_root_signature_desc_t::init_root_signature_desc(shader_info_t const& shader_info)
	{
		m_root_signature_desc = std::make_optional<D3D12_VERSIONED_ROOT_SIGNATURE_DESC>();
		
		(*m_root_signature_desc).Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
		(*m_root_signature_desc).Desc_1_1.NumParameters = static_cast<UINT>(std::size(m_root_parameters));
		(*m_root_signature_desc).Desc_1_1.pParameters = std::data(m_root_parameters);
		(*m_root_signature_desc).Desc_1_1.NumStaticSamplers = {};
		(*m_root_signature_desc).Desc_1_1.pStaticSamplers = {};
		(*m_root_signature_desc).Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;
		if (shader_info.get_meta_block().has_entry_for_vert()) (*m_root_signature_desc).Desc_1_1.Flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	}

	void d3d12_root_signature_desc_t::shut_root_signature_desc()
	{
		m_root_signature_desc = {};
	}
}
