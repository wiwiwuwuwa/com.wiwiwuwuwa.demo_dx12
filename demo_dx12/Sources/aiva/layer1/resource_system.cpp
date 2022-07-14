#include <pch.h>
#include <aiva/layer1/resource_system.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/ro_shader_compute.h>
#include <aiva/utils/asserts.h>

aiva::layer1::ResourceSystem::ResourceSystem(aiva::layer1::Engine const& engine) : mEngine{ engine }
{
	InitializeFactories();
}

aiva::layer1::ResourceSystem::~ResourceSystem()
{
	TerminateFactories();
}

std::shared_ptr<aiva::layer1::IResourceObject> aiva::layer1::ResourceSystem::GetResource(std::filesystem::path const& fileName)
{
	aiva::utils::Asserts::CheckBool(!fileName.empty());

	std::filesystem::path const fileExtension = fileName.extension();
	aiva::utils::Asserts::CheckBool(!fileExtension.empty());

	std::shared_ptr<aiva::layer1::IResourceObject> const resourceFromCache = GetResourceFromCache(fileName);
	if (resourceFromCache) return resourceFromCache;

	std::vector<std::byte> const binaryFromFile = GetBinaryFromFile(fileName);
	aiva::utils::Asserts::CheckBool(!binaryFromFile.empty());

	std::shared_ptr<aiva::layer1::IResourceObject> const resourceFromBinary = GetResourceFromBinary(fileExtension, binaryFromFile);
	aiva::utils::Asserts::CheckBool(resourceFromBinary);

	SetResourceToCache(fileName, resourceFromBinary);
	return resourceFromBinary;
}

std::shared_ptr<aiva::layer1::IResourceObject> aiva::layer1::ResourceSystem::GetResourceFromCache(std::filesystem::path const& fileName) const
{
	aiva::utils::Asserts::CheckBool(!fileName.empty());

	auto const resourceIter = mResources.find(fileName);
	if (resourceIter == mResources.end()) return {};

	auto const resourcePointer = resourceIter->second.lock();
	if (!resourcePointer) return {};

	return resourcePointer;
}

std::vector<std::byte> aiva::layer1::ResourceSystem::GetBinaryFromFile(std::filesystem::path const& fileName) const
{
	aiva::utils::Asserts::CheckBool(!fileName.empty());

	std::uintmax_t const fileSize = std::filesystem::file_size(fileName);
	aiva::utils::Asserts::CheckBool(fileSize > 0);

	std::basic_ifstream<std::byte> binaryStream{ fileName, std::ios_base::in | std::ios_base::binary };
	aiva::utils::Asserts::CheckBool(binaryStream);

	std::vector<std::byte> fileData{ fileSize };
	binaryStream.read(fileData.data(), fileSize);

	return fileData;
}

std::shared_ptr<aiva::layer1::IResourceObject> aiva::layer1::ResourceSystem::GetResourceFromBinary(std::filesystem::path const& extension, std::vector<std::byte> const& binary) const
{
	aiva::utils::Asserts::CheckBool(!extension.empty());
	aiva::utils::Asserts::CheckBool(!binary.empty());

	auto const factory = mFactories.find(extension);
	aiva::utils::Asserts::CheckBool(factory != mFactories.end());

	std::shared_ptr<aiva::layer1::IResourceObject> const resource = factory->second(mEngine, binary);
	aiva::utils::Asserts::CheckBool(resource);

	return resource;
}

void aiva::layer1::ResourceSystem::SetResourceToCache(std::filesystem::path const& fileName, std::shared_ptr<aiva::layer1::IResourceObject> const& resource)
{
	aiva::utils::Asserts::CheckBool(!fileName.empty());
	aiva::utils::Asserts::CheckBool(resource);

	mResources.insert_or_assign(fileName, resource);
}

void aiva::layer1::ResourceSystem::InitializeFactories()
{
	mFactories = {};
	RegisterFactory<aiva::layer1::RoShaderCompute>(".hlsl_cs");
}

void aiva::layer1::ResourceSystem::TerminateFactories()
{
	mFactories = {};
}
