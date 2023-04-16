#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>
#include <aiva2/shader_register_type.hpp>
#include <aiva2/shader_resource_frequency.hpp>
#include <aiva2/shader_resource_type.hpp>

namespace aiva2
{
	struct shader_info_for_resource_t final : public implements_t<shader_info_for_resource_t, engine_object_t>
	{
		// ------------------------------------------------

	public:
		shader_info_for_resource_t(engine_t& engine, std::string const& text);

		~shader_info_for_resource_t() override;

		// ------------------------------------------------

	public:
		auto get_text() const->std::string const&;

		auto has_text() const->bool;

	private:
		std::string m_text{};

		// ------------------------------------------------

	public:
		auto get_type() const->shader_resource_type_t;

	private:
		void init_type();

		void shut_type();

	private:
		shader_resource_type_t m_type{};

		// ------------------------------------------------

	public:
		auto get_name() const->std::string const&;
		
		auto has_name() const->bool;

	private:
		void init_name();

		void shut_name();

	private:
		std::string m_name{};

		// ------------------------------------------------

	public:
		auto get_struct_name() const->std::string const&;

	private:
		void init_struct_name();

		void shut_struct_name();

	private:
		std::string m_struct_name{};

		// ------------------------------------------------

	public:
		auto get_register_type() const->shader_register_type_t;

		auto get_register_index() const->size_t;
		
		auto get_register_space() const->size_t;

	private:
		void init_register();

		void shut_register();
		
	private:
		shader_register_type_t m_register_type{};

		size_t m_register_index{};
		
		size_t m_register_space{};

		// ------------------------------------------------

	public:
		auto get_frequency() const->shader_resource_frequency_t;

	private:
		void init_frequency();

		void shut_frequency();

	private:
		shader_resource_frequency_t m_frequency{};

		// ------------------------------------------------
	};
}
