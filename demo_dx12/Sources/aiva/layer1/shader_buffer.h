#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct ShaderStruct;
}

namespace aiva::layer1
{
	struct ShaderBuffer final : private boost::noncopyable, public std::enable_shared_from_this<ShaderBuffer>
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<ShaderBuffer> Create(TArgs&&... args);

	private:
		ShaderBuffer(std::shared_ptr<const ShaderStruct> const& referenceStruct);

	public:
		~ShaderBuffer();

	private:
		std::shared_ptr<const ShaderStruct> mReferenceStruct{};

	// ----------------------------------------------------
	// Serialization

	public:
		std::vector<std::byte> SerializeToBinary() const;

	// ----------------------------------------------------
	// Reference Structure

	public:
		std::size_t ByteStride() const;

	// ----------------------------------------------------
	// Shader Structures

	public:
		ShaderBuffer& Add(std::shared_ptr<ShaderStruct> const& shaderStruct);

		std::shared_ptr<const ShaderStruct> const& Get(std::size_t index) const;

		std::size_t Num() const;

	private:
		std::vector<std::shared_ptr<ShaderStruct>> mShaderStructs{};
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::ShaderBuffer> aiva::layer1::ShaderBuffer::Create(TArgs&&... args)
{
	return std::shared_ptr<ShaderBuffer>{new ShaderBuffer{ std::forward<TArgs>(args)... }};
}
