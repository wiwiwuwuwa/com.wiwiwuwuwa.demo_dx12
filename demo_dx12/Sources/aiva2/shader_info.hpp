#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct shader_info_t final : public implements_t<shader_info_t, engine_object_t>
	{
		// ------------------------------------------------

	public:
		shader_info_t(engine_t& engine, std::string const& text);

		~shader_info_t() override;

		// ------------------------------------------------

	public:
		auto get_text() const->std::string const&;

		auto has_text() const->bool;

	private:
		std::string m_text{};

		// ------------------------------------------------

	public:
		auto get_meta_block() const->shader_info_for_meta_t const&;

		auto has_meta_block() const->bool;

	private:
		void init_meta_block();

		void shut_meta_block();
		
	private:
		std::shared_ptr<shader_info_for_meta_t> m_meta_block{};
		
		// ------------------------------------------------

	public:
		auto get_code_block() const->shader_info_for_code_t const&;

		auto has_code_block() const->bool;

	private:
		void init_code_block();

		void shut_code_block();
		
	private:
		std::shared_ptr<shader_info_for_code_t> m_code_block{};

		// ------------------------------------------------
	};
}
