#include <pch.h>
#include <aiva/layer1/resource_system.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/ro_material_compute.h>
#include <aiva/layer1/ro_material_graphic.h>
#include <aiva/layer1/ro_scene_glb.h>
#include <aiva/layer1/ro_shader_compute.h>
#include <aiva/layer1/ro_shader_fragment.h>
#include <aiva/layer1/ro_shader_vertex.h>
#include <aiva/utils/asserts.h>

aiva::layer1::ResourceSystem::ResourceSystem(aiva::layer1::Engine const& engine) : mEngine{ engine }
{
	InitializeFactories();
}

aiva::layer1::ResourceSystem::~ResourceSystem()
{
	TerminateFactories();
}

std::shared_ptr<aiva::layer1::ICpuResource> aiva::layer1::ResourceSystem::GetResource(std::filesystem::path const& fileName)
{
	aiva::utils::Asserts::CheckBool(!fileName.empty());

	std::shared_ptr<aiva::layer1::ICpuResource> const resourceFromCache = GetResourceFromCache(fileName);
	if (resourceFromCache) return resourceFromCache;

	std::filesystem::path const fileExtension = fileName.extension();
	aiva::utils::Asserts::CheckBool(!fileExtension.empty());

	std::shared_ptr<aiva::layer1::ICpuResource> const resourceFromFactory = GetResourceFromFactory(fileExtension);
	aiva::utils::Asserts::CheckBool(resourceFromFactory);

	SetResourceToCache(fileName, resourceFromFactory);

	std::vector<std::byte> const binaryFromFile = GetBinaryFromFile(fileName);
	aiva::utils::Asserts::CheckBool(!binaryFromFile.empty());

	DeserealizeResourceFromBinary(resourceFromFactory, binaryFromFile);
	return resourceFromFactory;
}

std::shared_ptr<aiva::layer1::ICpuResource> aiva::layer1::ResourceSystem::GetResourceFromCache(std::filesystem::path const& fileName) const
{
	aiva::utils::Asserts::CheckBool(!fileName.empty());

	auto const resourceIter = mResources.find(fileName);
	if (resourceIter == mResources.end()) return {};

	auto const resourcePointer = resourceIter->second.lock();
	if (!resourcePointer) return {};

	return resourcePointer;
}

std::shared_ptr<aiva::layer1::ICpuResource> aiva::layer1::ResourceSystem::GetResourceFromFactory(std::filesystem::path const& fileExtension) const
{
	aiva::utils::Asserts::CheckBool(!fileExtension.empty());

	auto const factory = mFactories.find(fileExtension);
	aiva::utils::Asserts::CheckBool(factory != mFactories.end());

	std::shared_ptr<aiva::layer1::ICpuResource> const resource = factory->second(mEngine);
	aiva::utils::Asserts::CheckBool(resource);

	return resource;
}

void aiva::layer1::ResourceSystem::SetResourceToCache(std::filesystem::path const& fileName, std::shared_ptr<aiva::layer1::ICpuResource> const& resource)
{
	aiva::utils::Asserts::CheckBool(!fileName.empty());
	aiva::utils::Asserts::CheckBool(resource);

	mResources.insert_or_assign(fileName, resource);
}

std::vector<std::byte> aiva::layer1::ResourceSystem::GetBinaryFromFile(std::filesystem::path const& fileName) const
{
	aiva::utils::Asserts::CheckBool(!fileName.empty());

	std::filesystem::path const fullPath = std::filesystem::path{ "Assets" } / fileName;
	aiva::utils::Asserts::CheckBool(!fullPath.empty());

	std::uintmax_t const fileSize = std::filesystem::file_size(fullPath);
	aiva::utils::Asserts::CheckBool(fileSize > 0);

	std::basic_ifstream<std::byte> binaryStream{ fullPath, std::ios_base::in | std::ios_base::binary };
	aiva::utils::Asserts::CheckBool(binaryStream);

	std::vector<std::byte> fileData{ fileSize };
	binaryStream.read(fileData.data(), fileSize);

	return fileData;
}

void aiva::layer1::ResourceSystem::DeserealizeResourceFromBinary(std::shared_ptr<aiva::layer1::ICpuResource> const& resource, std::vector<std::byte> const& binary) const
{
	aiva::utils::Asserts::CheckBool(resource);
	aiva::utils::Asserts::CheckBool(!binary.empty());

	resource->DeserealizeFromBinary(binary);
}

void aiva::layer1::ResourceSystem::InitializeFactories()
{
	mFactories = {};
	RegisterFactory<aiva::layer1::RoMaterialCompute>(".mat_cs");
	RegisterFactory<aiva::layer1::RoMaterialGraphic>(".mat_gs");
	RegisterFactory<aiva::layer1::RoSceneGlb>(".scene_glb");
	RegisterFactory<aiva::layer1::RoShaderCompute>(".hlsl_cs");
	RegisterFactory<aiva::layer1::RoShaderFragment>(".hlsl_ps");
	RegisterFactory<aiva::layer1::RoShaderVertex>(".hlsl_vs");
}

void aiva::layer1::ResourceSystem::TerminateFactories()
{
	mFactories = {};
}
