#include <pch.h>
#include <aiva/layer1/ro_json.h>

#include <aiva/utils/asserts.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	RoJson::RoJson(EngineType const& engine) : AObject{}, ICpuResource{}, IObjectEngineable{ engine }
	{

	}

	RoJson::~RoJson()
	{

	}

	void RoJson::DeserealizeFromBinary(std::vector<std::byte> const& binaryData)
	{
		Asserts::CheckBool(!std::empty(binaryData), "Binary data is empty");
		mJson = nlohmann::json::parse(binaryData);
	}

	nlohmann::json const& RoJson::Json() const
	{
		return mJson;
	}
}
