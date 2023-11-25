#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/enum_utils.hpp>
#include <aiva2/shader_register_group_type.hpp>
#include <aiva2/shader_register_type.hpp>

namespace aiva2
{
	// ----------------------------------------------------

	constexpr void from_letter(std::string::value_type const in_val, shader_register_type_t& out_val)
	{
		if (in_val == 'b')
			{ out_val = shader_register_type_t::CONSTANT_BUFFER; return; }

		if (in_val == 't')
			{ out_val = shader_register_type_t::SHADER_RESOURCE; return; }

		if (in_val == 'u')
			{ out_val = shader_register_type_t::UNORDERED_ACCESS; return; }

		if (in_val == 's')
			{ out_val = shader_register_type_t::SAMPLER; return; }

		assert_t::check_bool(false, "unsupported shader register type letter");
		out_val = shader_register_type_t::UNKNOWN;
	}

	// ----------------------------------------------------

	constexpr auto to_letter(shader_register_type_t const type)
	{
		switch (type)
		{
		case shader_register_type_t::CONSTANT_BUFFER:
			return 'b';

		case shader_register_type_t::SHADER_RESOURCE:
			return 't';

		case shader_register_type_t::UNORDERED_ACCESS:
			return 'u';

		case shader_register_type_t::SAMPLER:
			return 's';

		default:
			assert_t::check_bool(false, "unsupported shader register type");
			return '\0';
		}
	}

	// ----------------------------------------------------
	
	constexpr auto to_group_type(shader_register_type_t const type)
	{
		switch (type)
		{
		case shader_register_type_t::CONSTANT_BUFFER:
			return shader_register_group_type_t::GENERAL;

		case shader_register_type_t::SHADER_RESOURCE:
			return shader_register_group_type_t::GENERAL;

		case shader_register_type_t::UNORDERED_ACCESS:
			return shader_register_group_type_t::GENERAL;

		case shader_register_type_t::SAMPLER:
			return shader_register_group_type_t::SAMPLER;

		default:
			assert_t::check_bool(false, "unsupported shader register type");
			return shader_register_group_type_t::UNKNOWN;
		}
	}

	// ----------------------------------------------------

	constexpr auto to_d3d12_descriptor_range_type(shader_register_type_t const type)
	{
		switch (type)
		{
		case shader_register_type_t::CONSTANT_BUFFER:
			return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;

		case shader_register_type_t::SHADER_RESOURCE:
			return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;

		case shader_register_type_t::UNORDERED_ACCESS:
			return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;

		case shader_register_type_t::SAMPLER:
			return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;

		default:
			assert_t::check_bool(false, "unsupported shader register type");
			return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		}
	}

	// ----------------------------------------------------

	constexpr auto to_d3d12_root_parameter_type(shader_register_type_t const type)
	{
		switch (type)
		{
		case shader_register_type_t::CONSTANT_BUFFER:
			return D3D12_ROOT_PARAMETER_TYPE_CBV;

		case shader_register_type_t::SHADER_RESOURCE:
			return D3D12_ROOT_PARAMETER_TYPE_SRV;

		case shader_register_type_t::UNORDERED_ACCESS:
			return D3D12_ROOT_PARAMETER_TYPE_UAV;

		default:
			assert_t::check_bool(false, "unsupported shader register type");
			return D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		}
	}

	// ----------------------------------------------------
}
