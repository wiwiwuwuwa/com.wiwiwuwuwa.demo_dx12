#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct resource_system_t final : public implements_t<resource_system_t, engine_object_t>
	{
		// ------------------------------------------------

	public:
		resource_system_t(engine_t& engine);

		~resource_system_t() override;

	public:
		template <typename t_resource_type>
		std::shared_ptr<t_resource_type> get_resource(std::filesystem::path const& resource_path);

	private:
		template <typename t_resource_type>
		std::shared_ptr<t_resource_type> get_resource_from_heap(std::filesystem::path const& resource_path);

		template <typename t_resource_type>
		std::shared_ptr<t_resource_type> get_resource_from_file(std::filesystem::path const& resource_path);

		template <typename t_resource_type>
		std::shared_ptr<t_resource_type> new_resource_from_path(std::filesystem::path const& resource_path);

		template <typename t_resource_type>
		std::shared_ptr<t_resource_type> new_resource_from_libr(std::filesystem::path const& resource_path);

		template <typename t_resource_type>
		std::shared_ptr<t_resource_type> new_resource_from_json(std::filesystem::path const& resource_path);

		template <typename t_resource_type>
		std::shared_ptr<t_resource_type> new_resource_from_data(std::filesystem::path const& resource_path);

	private:
		std::unordered_map<std::filesystem::path, std::weak_ptr<engine_object_t>> m_resources{};

		// ------------------------------------------------
	};
}

// --------------------------------------------------------

#include <aiva2/assert.hpp>
#include <aiva2/file_utils.hpp>
#include <aiva2/has_constructor.hpp>
#include <aiva2/resource_loader_library.hpp>

namespace aiva2
{
	template <typename t_resource_type>
	std::shared_ptr<t_resource_type> resource_system_t::get_resource(std::filesystem::path const& resource_path)
	{
		assert_t::check_bool(!std::empty(resource_path), "resource_path is not valid");

		auto const resource_from_heap = get_resource_from_heap<t_resource_type>(resource_path);
		if (resource_from_heap) return resource_from_heap;
		
		auto const resource_from_file = get_resource_from_file<t_resource_type>(resource_path);
		if (resource_from_file) return resource_from_file;
		
		assert_t::check_bool(false, "resource was not found");
		return {};
	}

	template <typename t_resource_type>
	std::shared_ptr<t_resource_type> resource_system_t::get_resource_from_heap(std::filesystem::path const& resource_path)
	{
		assert_t::check_bool(!std::empty(resource_path), "resource_path is not valid");

		auto const resource_iter = m_resources.find(resource_path);
		if (resource_iter == std::cend(m_resources)) return {};

		auto const resource_basic = (*resource_iter).second.lock();
		if (!resource_basic) { m_resources.erase(resource_iter); return {}; };
		
		auto const resource_specific = std::dynamic_pointer_cast<t_resource_type>((*resource_iter).second.lock());
		assert_t::check_bool(resource_specific, "resource_specific is not valid");
		
		return resource_specific;
	}

	template <typename t_resource_type>
	std::shared_ptr<t_resource_type> resource_system_t::get_resource_from_file(std::filesystem::path const& resource_path)
	{
		assert_t::check_bool(!std::empty(resource_path), "resource_path is not valid");

		auto const resource_specific = new_resource_from_path<t_resource_type>(resource_path);
		assert_t::check_bool(resource_specific, "resource_specific is not valid");

		m_resources.insert_or_assign(resource_path, resource_specific);
		return resource_specific;
	}

	template <typename t_resource_type>
	std::shared_ptr<t_resource_type> resource_system_t::new_resource_from_path(std::filesystem::path const& resource_path)
	{
		assert_t::check_bool(!std::empty(resource_path), "resource_path is not valid");

		if (auto const resource_from_libr = new_resource_from_libr<t_resource_type>(resource_path); resource_from_libr)
			return resource_from_libr;

		if (auto const resource_from_json = new_resource_from_json<t_resource_type>(resource_path); resource_from_json)
			return resource_from_json;

		if (auto const resource_from_data = new_resource_from_data<t_resource_type>(resource_path); resource_from_data)
			return resource_from_data;
		
		assert_t::check_bool(false, "resource was not found");
		return {};
	}

	template <typename t_resource_type>
	std::shared_ptr<t_resource_type> resource_system_t::new_resource_from_libr(std::filesystem::path const& resource_path)
	{
		assert_t::check_bool(!std::empty(resource_path), "resource_path is not valid");

		auto const basic_resource = resource_loader_library_t::load_resource(get_engine(), std::type_index{ typeid(t_resource_type) }, resource_path);
		if (!basic_resource) return {};

		auto const specific_resource = std::dynamic_pointer_cast<t_resource_type>(basic_resource);
		assert_t::check_bool(specific_resource, "(specific_resource) is not valid");

		return specific_resource;
	}

	template <typename t_resource_type>
	std::shared_ptr<t_resource_type> resource_system_t::new_resource_from_json(std::filesystem::path const& resource_path)
	{
		assert_t::check_bool(!std::empty(resource_path), "resource_path is not valid");

		if constexpr (has_constructor<t_resource_type, engine_t&, nlohmann::json const&>())
		{
			auto const resource_json = file_utils_t::load_json_from_file(resource_path);
			assert_t::check_bool(!std::empty(resource_json), "resource_json is not valid");

			return std::make_shared<t_resource_type>(get_engine(), resource_json);
		}
		else
		{
			return {};
		}
	}

	template <typename t_resource_type>
	std::shared_ptr<t_resource_type> resource_system_t::new_resource_from_data(std::filesystem::path const& resource_path)
	{
		assert_t::check_bool(!std::empty(resource_path), "resource_path is not valid");

		if constexpr (has_constructor<t_resource_type, engine_t&, boost::span<std::byte const> const&>())
		{
			auto const resource_data = file_utils_t::load_data_from_file(resource_path);
			assert_t::check_bool(!std::empty(resource_data), "resource_data is not valid");

			return std::make_shared<t_resource_type>(get_engine(), resource_data);
		}
		else
		{
			return {};
		}
	}
}
