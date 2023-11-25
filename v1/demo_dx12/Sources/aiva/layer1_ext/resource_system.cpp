#include <pch.h>
#include <aiva/layer1_ext/resource_system.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;

	ResourceSystem::ResourceSystem(aiva::layer1::Engine& engine) : mEngine{ engine }
	{

	}

	ResourceSystem::~ResourceSystem()
	{

	}

	std::shared_ptr<aiva::utils_ext::IObject>  ResourceSystem::GetResourceFromHeap(std::filesystem::path const& fileName) const
	{
		aiva::utils::Asserts::CheckBool(!std::empty(fileName), "File name is not valid");

		auto const resourceItr = mResourceHeap.find(fileName);
		if (resourceItr == std::cend(mResourceHeap)) return {};

		return resourceItr->second.lock();
	}

	std::vector<std::byte> ResourceSystem::GetBinaryFromFile(std::filesystem::path const& fileName) const
	{
		Asserts::CheckBool(!std::empty(fileName), "File name is not valid");

		auto const fullPath = std::filesystem::path{ "Assets" } / fileName;
		Asserts::CheckBool(!std::empty(fullPath), "Full path is not valid");

		auto const fileSize = std::filesystem::file_size(fullPath);
		Asserts::CheckBool(fileSize > 0, "File size is not valid");

		auto fileStream = std::basic_ifstream<std::byte>{ fullPath, std::ios_base::in | std::ios_base::binary };
		Asserts::CheckBool(fileStream, "File stream is not valid");

		auto fileBinary = std::vector<std::byte>{ fileSize };
		fileStream.read(fileBinary.data(), fileSize);

		return fileBinary;
	}

	nlohmann::json ResourceSystem::GetJsonFromFile(std::filesystem::path const& fileName) const
	{
		auto const fileBinary = GetBinaryFromFile(fileName);
		Asserts::CheckBool(!std::empty(fileBinary), "File binary is not valid");

		return nlohmann::json::parse(fileBinary);
	}

	void ResourceSystem::SetResourceToHeap(std::filesystem::path const& fileName, std::shared_ptr<aiva::utils_ext::IObject> const& resourcePtr)
	{
		Asserts::CheckBool(!std::empty(fileName), "File name is not valid");
		Asserts::CheckBool(resourcePtr, "Resource ptr is not valid");

		mResourceHeap.insert_or_assign(fileName, resourcePtr);
	}
}
