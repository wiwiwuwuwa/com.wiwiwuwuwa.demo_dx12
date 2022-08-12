#pragma once
#include <pch.h>

#include <aiva/layer1/i_cpu_resource.h>
#include <aiva/layer1/i_object_engineable.h>
#include <aiva/layer1/ro_json_fwd.h>
#include <aiva/utils/a_object.h>

namespace aiva::layer1
{
	struct RoJson final : public aiva::utils::AObject, public ICpuResource, public IObjectEngineable
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	public:
		template <typename... Args>
		static RoJsonTypeShared Create(Args&&... args);

	protected:
		RoJson(EngineType const& engine);

	public:
		~RoJson() override;

	// ----------------------------------------------------
	// ICpuResource

	public:
		void DeserealizeFromBinary(std::vector<std::byte> const& binaryData) override;

	// ----------------------------------------------------
	// Data

	public:
		nlohmann::json const& Json() const;

	private:
		nlohmann::json mJson{};
	};
}

// --------------------------------------------------------

#include <aiva/utils/object_utils.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	template <typename... Args>
	RoJsonTypeShared RoJson::Create(Args&&... args)
	{
		return NewObject<RoJsonType>(std::forward<Args>(args)...);
	}
}
