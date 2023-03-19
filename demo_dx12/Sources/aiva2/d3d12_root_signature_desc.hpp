#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/object.hpp>
#include <aiva2/shader_register_group_type.hpp>

namespace aiva2
{
	struct d3d12_root_signature_desc_t final : public implements_t<d3d12_root_signature_desc_t, object_t>
	{
		// ------------------------------------------------

	public:
		d3d12_root_signature_desc_t(shader_info_t const& shader_info);
		
		~d3d12_root_signature_desc_t() override;

		// ------------------------------------------------

	private:
		void init_descriptor_ranges(shader_info_t const& shader_info);

		void shut_descriptor_ranges();

	private:
		std::map<shader_register_group_type_t, std::vector<D3D12_DESCRIPTOR_RANGE1>> m_descriptor_ranges{};
		
		// ------------------------------------------------

	private:
		void init_root_parameters(shader_info_t const& shader_info);

		void shut_root_parameters();
		
	private:
		std::vector<D3D12_ROOT_PARAMETER1> m_root_parameters{};

		// ------------------------------------------------

	public:
		auto get_root_signature_desc() const->D3D12_VERSIONED_ROOT_SIGNATURE_DESC const&;

	private:
		void init_root_signature_desc(shader_info_t const& shader_info);

		void shut_root_signature_desc();

	private:
		std::optional<D3D12_VERSIONED_ROOT_SIGNATURE_DESC> m_root_signature_desc{};

		// ------------------------------------------------
	};
}
