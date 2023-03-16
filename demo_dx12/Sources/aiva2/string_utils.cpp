#include <pch.h>
#include <aiva2/string_utils.hpp>

namespace aiva2
{
	auto string_utils_t::to_string(boost::span<std::byte const> const& span) -> std::string
	{
		return std::string{ reinterpret_cast<std::string::const_pointer>(std::data(span)), std::size(span) };
	}

	auto string_utils_t::to_string(winrt::com_ptr<ID3DBlob> const& blob) -> std::string
	{
		return std::string{ reinterpret_cast<std::string::const_pointer>(blob->GetBufferPointer()), blob->GetBufferSize() };
	}
}
