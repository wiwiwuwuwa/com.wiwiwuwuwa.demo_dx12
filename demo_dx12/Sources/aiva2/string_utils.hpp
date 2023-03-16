#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
	struct string_utils_t final
	{
		// ------------------------------------------------

	private:
		string_utils_t() = delete;

	public:
		static auto to_string(boost::span<std::byte const> const& span) -> std::string;

		static auto to_string(winrt::com_ptr<ID3DBlob> const& blob) -> std::string;

		// ------------------------------------------------
	};
}
