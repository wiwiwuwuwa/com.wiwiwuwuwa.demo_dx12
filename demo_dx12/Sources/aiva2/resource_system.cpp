#include <pch.h>
#include <aiva2/resource_system.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/string_view_utils.hpp>

namespace aiva2
{
	resource_system_t::resource_system_t(engine_t& engine)
		: impl_type{ engine }
	{
		
	}

	resource_system_t::~resource_system_t()
	{
		
	}
	
	auto resource_system_t::get_data_from_file(std::filesystem::path const& resource_path) const -> std::vector<std::byte>
	{
		assert_t::check_bool(!std::empty(resource_path), "resource_path is empty");

		auto const full_path = std::filesystem::path{ "Assets" } / resource_path;
		assert_t::check_bool(!std::empty(full_path), "full_path is empty");
		assert_t::check_bool(std::filesystem::exists(full_path), "full_path does not exist");

		auto const file_size_as_uintmax = std::filesystem::file_size(full_path);
		assert_t::check_bool(file_size_as_uintmax > 0, "file_size_as_uintmax is not valid");

		auto const file_size_as_size = static_cast<size_t>(file_size_as_uintmax);
		assert_t::check_bool(file_size_as_size > 0, "file_size_as_size is not valid");
		assert_t::check_bool(static_cast<uintmax_t>(file_size_as_size) == file_size_as_uintmax, "file_size_as_size is not valid");
		
		auto const file_size_as_streamsize = static_cast<std::streamsize>(file_size_as_size);
		assert_t::check_bool(file_size_as_streamsize > 0, "file_size_as_streamsize is not valid");
		assert_t::check_bool(static_cast<size_t>(file_size_as_streamsize) == file_size_as_size, "file_size_as_streamsize is not valid");

		auto file_stream = std::basic_ifstream<std::byte>{ full_path, std::ios_base::in | std::ios_base::binary };
		assert_t::check_bool(file_stream, "file_stream is not valid");
		assert_t::check_bool(file_stream.is_open(), "file_stream is not open");

		auto file_binary = std::vector<std::byte>{ file_size_as_size };
		assert_t::check_bool(std::size(file_binary) == file_size_as_size, "file_binary is not valid");

		file_stream.read(std::data(file_binary), file_size_as_streamsize);
		assert_t::check_bool(file_stream, "file_stream is not valid");
		assert_t::check_bool(file_stream.good(), "file_stream is not good");
		assert_t::check_bool(file_stream.gcount() == file_size_as_streamsize, "file_stream is not valid");

		return file_binary;
	}

	auto resource_system_t::get_json_from_file(std::filesystem::path const& resource_path) const -> nlohmann::json
	{
		auto const file_data = get_data_from_file(resource_path);
		assert_t::check_bool(!std::empty(file_data), "file_data is not valid");
		
		auto const file_view = string_view_utils_t::to_string_view(file_data);
		assert_t::check_bool(!std::empty(file_view), "file_view is not valid");

		return nlohmann::json::parse(file_view);
	}
}
