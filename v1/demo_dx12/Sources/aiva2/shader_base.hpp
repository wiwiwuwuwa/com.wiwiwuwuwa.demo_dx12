#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct shader_base_t : public implements_t<shader_base_t, engine_object_t>
	{
		// ------------------------------------------------

	protected:
		shader_base_t(engine_t& engine, boost::span<std::byte const> const& binary);

		~shader_base_t() override;

		// ------------------------------------------------

	public:
		auto get_info() const->shader_info_t const&;

	private:
		void init_info(boost::span<std::byte const> const& binary);

		void shut_info();

	private:
		std::shared_ptr<shader_info_t> m_info{};

		// ------------------------------------------------

	public:
		auto get_root_signature_ref() const->ID3D12RootSignature&;

		auto get_root_signature_ptr() const->winrt::com_ptr<ID3D12RootSignature> const&;

	private:
		void init_root_signature();

		void shut_root_signature();

	private:
		winrt::com_ptr<ID3D12RootSignature> m_root_signature{};

		// ------------------------------------------------
	};
}
