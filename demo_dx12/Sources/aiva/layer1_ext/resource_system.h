#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct ResourceSystem final : public aiva::utils_ext::TObject<ResourceSystem>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY();

	protected:
		ResourceSystem(aiva::layer1::Engine& engine);

	public:
		~ResourceSystem() override;

	// ----------------------------------------------------
	// Fields

	public:
		M_OBJECT_FIELD_REF_3(aiva::layer1::Engine, Engine, public);

	// ----------------------------------------------------
	// Resources

	public:
		template <typename TResourceType>
		std::shared_ptr<TResourceType> GetResource(std::filesystem::path const& fileName);

	private:
		template <typename TResourceType>
		std::shared_ptr<TResourceType> GetResourceFromHeap(std::filesystem::path const& fileName) const;

		template <typename TResourceType>
		std::shared_ptr<TResourceType> GetResourceFromFile(std::filesystem::path const& fileName) const;

		std::vector<std::byte> GetBinaryFromFile(std::filesystem::path const& fileName) const;

		nlohmann::json GetJsonFromFile(std::filesystem::path const& fileName) const;

		void SetResourceToHeap(std::filesystem::path const& fileName, std::shared_ptr<aiva::utils_ext::IObject> const& resourcePtr);

	private:
		std::unordered_map<std::filesystem::path, std::weak_ptr<aiva::utils_ext::IObject>> mResourceHeap{};
	};
}

// --------------------------------------------------------

#include <aiva/layer1/engine.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/i_serializable_binary.h>
#include <aiva/utils_ext/i_serializable_json.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	template <typename TResourceType>
	std::shared_ptr<TResourceType> ResourceSystem::GetResource(std::filesystem::path const& fileName)
	{
		aiva::utils::Asserts::CheckBool(!std::empty(fileName), "File name is not valid");

		auto const resourceFromHeap = GetResourceFromHeap<TResourceType>(fileName);
		if (resourceFromHeap) return resourceFromHeap;

		auto const resourceFromFile = GetResourceFromFile<TResourceType>(fileName);
		aiva::utils::Asserts::CheckBool(resourceFromFile, "Resource from file is not valid");

		SetResourceToHeap(fileName, resourceFromFile);
		return resourceFromFile;
	}

	template <typename TResourceType>
	std::shared_ptr<TResourceType> ResourceSystem::GetResourceFromHeap(std::filesystem::path const& fileName) const
	{
		aiva::utils::Asserts::CheckBool(!std::empty(fileName), "File name is not valid");

		auto const resourceItr = mResourceHeap.find(fileName);
		if (resourceItr == std::end(mResourceHeap)) return {};

		auto const basicResourcePtr = resourceItr->second.lock();
		if (!basicResourcePtr) return {};

		auto const specificResourcePtr = std::dynamic_pointer_cast<TResourceType>(basicResourcePtr);
		aiva::utils::Asserts::CheckBool(specificResourcePtr, "Specific resource ptr is not valid");

		return specificResourcePtr;
	}

	template <typename TResourceType>
	std::shared_ptr<TResourceType> ResourceSystem::GetResourceFromFile(std::filesystem::path const& fileName) const
	{
		aiva::utils::Asserts::CheckBool(!std::empty(fileName), "File name is not valid");

		auto const resourcePtr = aiva::utils_ext::NewObject<TResourceType>(Engine());
		aiva::utils::Asserts::CheckBool(resourcePtr, "Resource ptr is not valid");

		if constexpr (std::is_base_of_v<aiva::utils_ext::ISerializableJson, TResourceType>)
		{
			auto const resourceJsn = GetJsonFromFile(fileName);
			aiva::utils::Asserts::CheckBool(!std::empty(resourceJsn), "Resource jsn is not valid");

			resourcePtr->DeserealizeFromJson(resourceJsn);
		}
		else
		{
			auto const resourceBin = GetBinaryFromFile(fileName);
			aiva::utils::Asserts::CheckBool(!std::empty(resourceBin), "Resource bin is not valid");

			resourcePtr->DeserealizeFromBinary(resourceBin);
		}

		return resourcePtr;
	}
}
