#include <pch.h>
#include <aiva/layer1/ro_material_compute.h>

#include <aiva/layer1/engine.h>
#include <aiva/utils/asserts.h>

aiva::layer1::RoMaterialCompute::RoMaterialCompute(aiva::layer1::Engine const& engine) : mEngine{ engine }
{

}


aiva::layer1::RoMaterialCompute::~RoMaterialCompute()
{

}

void aiva::layer1::RoMaterialCompute::Deserealize(std::vector<std::byte> const& binaryData)
{
	aiva::utils::Asserts::CheckBool(!binaryData.empty());
	BeginChanges();
	EndChanges();
}

aiva::layer1::RoMaterialCompute& aiva::layer1::RoMaterialCompute::BeginChanges()
{
	mChangesCounter.IncrementChanges();
	return *this;
}

aiva::layer1::RoMaterialCompute& aiva::layer1::RoMaterialCompute::EndChanges()
{
	mChangesCounter.DecrementChanges();
	return *this;
}

