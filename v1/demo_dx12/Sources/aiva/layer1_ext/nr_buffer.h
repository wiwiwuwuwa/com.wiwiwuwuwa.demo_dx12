#pragma once
#include <pch.h>

#include <aiva/layer1/e_buffer_memory_type.h>
#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/nr_buffer_fwd.h>
#include <aiva/layer1_ext/ut_resource_barriers.h>
#include <aiva/layer1_ext/ut_resource_barriers_emitter.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_val_dirty_flags_eager.h>
#include <aiva/utils_ext/m_object_field_val_dirty_flags_lazy.h>
#include <aiva/utils_ext/m_object_field_var_custom.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct NrBuffer final : public aiva::utils_ext::TObject<NrBuffer>, public UtResourceBarriersEmitter
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(NrBuffer);

	protected:
		NrBuffer(aiva::layer1::Engine& engine);

	public:
		~NrBuffer() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER();

	private:
		void InitGenericFields();

		void ShutGenericFields();

	private:
		void GenericFields_When_ParamsFields_OnDirtied();

		void GenericFields_When_ObjectFields_OnDirtied();

	// ----------------------------------------------------
	// Params Fields

	public:
		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_LAZY_2(DirtyFlagsForParamsGetter, private);

		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER_2(DirtyFlagsForParamsSetter, private);

	public:
		M_OBJECT_FIELD_VAR_CUSTOM_4(aiva::layer1::EBufferMemoryType, MemoryType, public, public);

		M_OBJECT_FIELD_VAR_CUSTOM_4(std::size_t, Size, public, public);

		M_OBJECT_FIELD_VAR_CUSTOM_4(bool, SupportShaderAtomics, public, public);

		M_OBJECT_FIELD_VAR_CUSTOM_4(bool, SupportUnorderedAccess, public, public);

	private:
		void InitParamsFields();

		void ShutParamsFields();

	private:
		void ParamsFields_When_ObjectFields_OnDirtied();

		void ParamsFields_When_ParamsFields_OnFlushed();

	private:
		void RefreshParamsFromObject();

	// ----------------------------------------------------
	// Object Fields

	public:
		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_LAZY_2(DirtyFlagsForObjectGetter, private);

		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER_2(DirtyFlagsForObjectSetter, private);

	public:
		M_OBJECT_FIELD_VAR_CUSTOM_4(winrt::com_ptr<ID3D12Resource>, Object, public, public);

	private:
		void InitObjectFields();

		void ShutObjectFields();

	private:
		void ObjectFields_When_ParamsFields_OnDirtied();

		void ObjectFields_When_ObjectFields_OnFlushed();

	private:
		void RefreshObjectFromParams();

	// ----------------------------------------------------
	// CPU Reading/Writing Fields

	public:
		void WriteBinaryData(boost::span<const std::byte> const& binaryData);

	// ----------------------------------------------------
	// Resource Barriers Emitter

	public:
		M_OBJECT_FIELD_VAR_3(private, UtResourceBarriers, ResourceBarriers);

	public:
		std::vector<D3D12_RESOURCE_BARRIER> TransiteResourceBarriers(std::optional<std::size_t> const subResource, D3D12_RESOURCE_STATES const desiredState) override;
	};
}
