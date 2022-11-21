#include <pch.h>
#include <aiva/layer1_ext/shader_struct_register_info.h>

#include <aiva/layer1_ext/shader_meta_for_register.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;

	ShaderStructRegisterInfo::ShaderStructRegisterInfo()
	{

	}

	ShaderStructRegisterInfo::ShaderStructRegisterInfo(std::shared_ptr<ShaderMetaForRegister> const& registerMeta)
	{
		Asserts::CheckBool(registerMeta, "Register meta is not valid");

		Value(registerMeta->Value());
		Index(registerMeta->Index());
		Space(registerMeta->Space());
	}

	bool ShaderStructRegisterInfo::operator<(ShaderStructRegisterInfo const& other) const
	{
		if (mValue != other.mValue)
		{
			return mValue < other.mValue;
		}

		if (mIndex != other.mIndex)
		{
			return mIndex < other.mIndex;
		}

		if (mSpace != other.mSpace)
		{
			return mSpace < other.mSpace;
		}

		return false;
	}

	std::vector<ShaderStructRegisterInfo> ShaderStructRegisterInfo::Convert(std::vector<std::shared_ptr<ShaderMetaForRegister>> const& registersMeta)
	{
		auto registersInfo = std::vector<ShaderStructRegisterInfo>{};

		for (auto const& registerMeta : registersMeta)
		{
			Asserts::CheckBool(registerMeta, "Register meta is not valid");
			registersInfo.emplace_back(registerMeta);
		}

		return registersInfo;
	}
}
