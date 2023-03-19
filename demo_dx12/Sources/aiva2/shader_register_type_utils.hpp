#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/enum_utils.hpp>
#include <aiva2/shader_register_group_type.hpp>
#include <aiva2/shader_register_type.hpp>

namespace aiva2
{
	// ----------------------------------------------------

	constexpr auto from_letter(std::string::value_type const letter)
	{
		switch (letter)
		{
		case 'b':
			return shader_register_type_t::CONSTANT_BUFFER;

		case 't':
			return shader_register_type_t::SHADER_RESOURCE;

		case 'u':
			return shader_register_type_t::UNORDERED_ACCESS;

		case 's':
			return shader_register_type_t::SAMPLER;

		default:
			assert_t::check_bool(false, "unsupported shader register type letter");
			return shader_register_type_t::UNKNOWN;
		}
	}

	template <std::string::value_type t_letter>
	constexpr auto from_letter()
	{
		return from_letter(t_letter);
	}

	template <std::string::value_type t_letter>
	struct from_letter_t : public std::integral_constant<shader_register_type_t, from_letter<t_letter>> {};

	template <std::string::value_type t_letter>
	constexpr auto from_letter_v = from_letter_t<t_letter>::value;

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

	template <shader_register_type_t t_type>
	constexpr auto to_letter()
	{
		return to_letter(t_type);
	}

	template <shader_register_type_t t_type>
	struct to_letter_t : public std::integral_constant<std::string::value_type, to_letter<t_type>> {};

	template <shader_register_type_t t_type>
	constexpr auto to_letter_v = to_letter_t<t_type>::value;

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

	template <shader_register_type_t t_type>
	constexpr auto to_group_type()
	{
		return to_group_type(t_type);
	}

	template <shader_register_type_t t_type>
	struct to_group_type_t : public std::integral_constant<shader_register_group_type_t, to_group_type<t_type>> {};
	
	template <shader_register_type_t t_type>
	constexpr auto to_group_type_v = to_group_type_t<t_type>::value;

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

	template <shader_register_type_t t_type>
	constexpr auto to_d3d12_descriptor_range_type()
	{
		return to_d3d12_descriptor_range_type(t_type);
	}

	template <shader_register_type_t t_type>
	struct to_d3d12_descriptor_range_type_t : public std::integral_constant<D3D12_DESCRIPTOR_RANGE_TYPE, to_d3d12_descriptor_range_type<t_type>> {};
	
	template <shader_register_type_t t_type>
	constexpr auto to_d3d12_descriptor_range_type_v = to_d3d12_descriptor_range_type_t<t_type>::value;

	// ----------------------------------------------------

	constexpr auto to_d3d12_root_descriptor_parameter_type(shader_register_type_t const type)
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

	template <shader_register_type_t t_type>
	constexpr auto to_d3d12_root_descriptor_parameter_type()
	{
		return to_d3d12_root_descriptor_parameter_type(t_type);
	}

	template <shader_register_type_t t_type>
	struct to_d3d12_root_descriptor_parameter_type_t : public std::integral_constant<D3D12_ROOT_PARAMETER_TYPE, to_d3d12_root_descriptor_parameter_type<t_type>> {};

	template <shader_register_type_t t_type>
	constexpr auto to_d3d12_root_descriptor_parameter_type_v = to_d3d12_root_descriptor_parameter_type_t<t_type>::value;

	// ----------------------------------------------------
}
