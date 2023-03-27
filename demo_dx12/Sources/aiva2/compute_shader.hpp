#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/shader_base.hpp>

namespace aiva2
{
	struct compute_shader_t final : public implements_t<compute_shader_t, shader_base_t>
	{
		// ------------------------------------------------
		
	public:
		compute_shader_t(engine_t& engine, boost::span<std::byte const> const& binary);
		
		~compute_shader_t() override;

		// ------------------------------------------------

	private:
		auto get_bytecode_for_comp() const->ID3DBlob&;

	private:
		void init_bytecode_for_comp();

		void shut_bytecode_for_comp();

	private:
		winrt::com_ptr<ID3DBlob> m_bytecode_for_comp{};

		// ------------------------------------------------

	public:
		auto get_pipeline_state_ref() const->ID3D12PipelineState&;

		auto get_pipeline_state_ptr() const->winrt::com_ptr<ID3D12PipelineState> const&;

	private:
		void init_pipeline_state();

		void shut_pipeline_state();
		
	private:
		winrt::com_ptr<ID3D12PipelineState> m_pipeline_state{};

		// ------------------------------------------------
	};
}
