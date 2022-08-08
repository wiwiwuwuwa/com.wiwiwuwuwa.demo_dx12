#pragma once
#include <pch.h>

#include <aiva/utils/a_object.h>
#include <aiva/utils/i_object_changeable.h>

namespace aiva::layer1
{
	struct ShaderStruct;
}

namespace aiva::layer1
{
	struct ShaderBuffer final : public aiva::utils::AObject, public aiva::utils::IObjectChangeable
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		ShaderBuffer();

	public:
		~ShaderBuffer() override;

	// ----------------------------------------------------
	// Reference Struct

	public:
		std::shared_ptr<const ShaderStruct> const& Struct() const;

		ShaderBuffer& Struct(std::shared_ptr<const ShaderStruct> const& referenceStruct);

	private:
		std::shared_ptr<const ShaderStruct> mStruct{};

	// ----------------------------------------------------
	// Shader Structures

	public:
		ShaderBuffer& Add(std::shared_ptr<const ShaderStruct> const& shaderStruct);

		std::shared_ptr<const ShaderStruct> const& Get(std::size_t index) const;

		std::size_t Num() const;

	private:
		std::vector<std::shared_ptr<const ShaderStruct>> mShaderStructs{};

	// ----------------------------------------------------
	// Serialization

	public:
		std::size_t ByteStride() const;

		std::vector<std::byte> SerializeToBinary() const;
	};
}
