#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct shader_info_for_code_t final : public implements_t<shader_info_for_code_t, engine_object_t>
	{
		// ------------------------------------------------

	public:
		shader_info_for_code_t(engine_t& engine, std::string const& text);

		~shader_info_for_code_t() override;

		// ------------------------------------------------

	public:
		auto get_text() const->std::string const&;

		auto has_text() const->bool;

	private:
		std::string m_text{};

		// ------------------------------------------------

	public:
		auto get_struct_ref(size_t const index) const->shader_info_for_struct_t const&;

		auto get_struct_ref(std::string_view const& name) const->shader_info_for_struct_t const&;

		auto get_struct_ptr(size_t const index) const->std::shared_ptr<shader_info_for_struct_t const>;

		auto get_struct_ptr(std::string_view const& name) const->std::shared_ptr<shader_info_for_struct_t const>;

		auto num_struct() const->size_t;

	private:
		void init_structs();

		void shut_structs();

	private:
		std::vector<std::shared_ptr<shader_info_for_struct_t>> m_structs{};

		// ------------------------------------------------

	public:
		auto get_resource_ref(size_t const index) const->shader_info_for_resource_t const&;

		auto get_resource_ptr(size_t const index) const->std::shared_ptr<shader_info_for_resource_t const> const&;
		
		auto num_resource() const->size_t;

	private:
		void init_resources();

		void shut_resources();

	private:
		std::vector<std::shared_ptr<shader_info_for_resource_t>> m_resources{};

		// ------------------------------------------------

	public:
		auto get_func_ref(size_t const index) const->shader_info_for_func_t const&;

		auto get_func_ref(std::string_view const& name) const->shader_info_for_func_t const&;

		auto get_func_ptr(size_t const index) const->std::shared_ptr<shader_info_for_func_t const>;

		auto get_func_ptr(std::string_view const& name) const->std::shared_ptr<shader_info_for_func_t const>;

		auto num_func() const->size_t;

	private:
		void init_funcs();

		void shut_funcs();

	private:
		std::vector<std::shared_ptr<shader_info_for_func_t>> m_funcs{};

		// ------------------------------------------------

	public:
		auto get_cbuffer_struct_ptr() const->std::shared_ptr<shader_info_for_struct_t const>;

		auto get_cbuffer_struct_ref() const->shader_info_for_struct_t const&;

	public:
		auto get_cbuffer_resource_ptr() const->std::shared_ptr<shader_info_for_resource_t const>;

		auto get_cbuffer_resource_ref() const->shader_info_for_resource_t const&;

	public:
		static constexpr const auto C_BUFFER_RESOURCE_NAME = "o_cbuffer";

		// ------------------------------------------------
	};
}
