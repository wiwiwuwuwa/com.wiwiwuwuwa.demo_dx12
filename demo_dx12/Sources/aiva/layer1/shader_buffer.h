#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct ShaderStruct;
}

namespace aiva::utils
{
	template <typename, typename>
	struct TCacheUpdater;
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
		ShaderBuffer();

	public:
		~ShaderBuffer();

	// ----------------------------------------------------
	// Cache Refresh

	public:
		enum class EDirtyFlags
		{
			None = 0,
			All = 1,
		};

		using CacheUpdaterType = aiva::utils::TCacheUpdater<ShaderBuffer, EDirtyFlags>;

	public:
		CacheUpdaterType& CacheUpdater() const;

	private:
		void InitializeCacheUpdater();

		void TerminateCacheUpdater();

	private:
		std::unique_ptr<CacheUpdaterType> mCacheUpdater{};

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

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::ShaderBuffer> aiva::layer1::ShaderBuffer::Create(TArgs&&... args)
{
	return std::shared_ptr<ShaderBuffer>{new ShaderBuffer{ std::forward<TArgs>(args)... }};
}
