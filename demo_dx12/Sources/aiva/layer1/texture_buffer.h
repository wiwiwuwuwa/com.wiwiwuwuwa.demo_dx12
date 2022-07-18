#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct TextureBuffer final : private boost::noncopyable, public std::enable_shared_from_this<TextureBuffer>
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<aiva::layer1::TextureBuffer> Create(TArgs&&... args);

	private:
		TextureBuffer(aiva::layer1::Engine const& engine);

	public:
		~TextureBuffer();

	private:
		aiva::layer1::Engine const& mEngine;
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::TextureBuffer> aiva::layer1::TextureBuffer::Create(TArgs&&... args)
{
	return std::shared_ptr<aiva::layer1::TextureBuffer>{new aiva::layer1::TextureBuffer{ std::forward<TArgs>(args)... }};
}
