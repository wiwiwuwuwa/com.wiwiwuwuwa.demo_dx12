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

	private:
		auto get_data_from_file(std::filesystem::path const& resource_path) const -> std::vector<std::byte>;

		auto get_json_from_file(std::filesystem::path const& resource_path) const -> nlohmann::json;

	private:
		std::unordered_map<std::filesystem::path, std::weak_ptr<engine_object_t>> m_resources{};

		// ------------------------------------------------
	};
}

// --------------------------------------------------------

#include <aiva2/has_constructor.hpp>

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

		if constexpr (has_constructor<t_resource_type, engine_t&, nlohmann::json const&>())
		{
			auto const resource_json = get_json_from_file(resource_path);
			assert_t::check_bool(!std::empty(resource_json), "resource_json is not valid");

			return std::make_shared<t_resource_type>(get_engine(), resource_json);
		}
		else if constexpr (has_constructor<t_resource_type, engine_t&, boost::span<std::byte const> const&>())
		{
			auto const resource_data = get_data_from_file(resource_path);
			assert_t::check_bool(!std::empty(resource_data), "resource_data is not valid");

			return std::make_shared<t_resource_type>(get_engine(), resource_data);
		}
		else
		{
			return {};
		}
	}
}
