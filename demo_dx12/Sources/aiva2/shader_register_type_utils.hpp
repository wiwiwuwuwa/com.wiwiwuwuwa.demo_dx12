#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/enum_utils.hpp>
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
}
