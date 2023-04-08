#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/buffer_format.hpp>
#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct shader_info_for_meta_t final : public implements_t<shader_info_for_meta_t, engine_object_t>
	{
		// ------------------------------------------------

	public:
		shader_info_for_meta_t(engine_t& engine, std::string const& text);

		~shader_info_for_meta_t() override;

		// ------------------------------------------------

	public:
		auto get_text() const->std::string const&;

		auto has_text() const->bool;

	private:
		std::string m_text{};

		// ------------------------------------------------

	public:
		auto get_entry_for_comp() const->std::string const&;

		auto has_entry_for_comp() const->bool;

	private:
		void init_entry_for_comp();
		
		void shut_entry_for_comp();

	private:
		std::string m_entry_for_comp{};

		// ------------------------------------------------

	public:
		auto get_entry_for_vert() const->std::string const&;

		auto has_entry_for_vert() const->bool;

	private:
		void init_entry_for_vert();

		void shut_entry_for_vert();

	private:
		std::string m_entry_for_vert{};

		// ------------------------------------------------

	public:
		auto get_entry_for_frag() const->std::string const&;
		
		auto has_entry_for_frag() const->bool;
		
	private:
		void init_entry_for_frag();
		
		void shut_entry_for_frag();

	private:
		std::string m_entry_for_frag{};

		// ------------------------------------------------

	public:
		auto get_depth_stencil_format() const->buffer_format_t;

		auto has_depth_stencil_format() const->bool;

	private:
		void init_depth_stencil_format();

		void shut_depth_stencil_format();

	private:
		std::optional<buffer_format_t> m_depth_stencil_format{};

		// ------------------------------------------------
	};
}
