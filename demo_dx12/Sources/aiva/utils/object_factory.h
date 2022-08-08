#pragma once
#include <pch.h>

namespace aiva::utils
{
	struct ObjectFactory final
	{
	// ----------------------------------------------------
	// Main

	private:
		ObjectFactory() = delete;

	// ----------------------------------------------------
	// Factory

	public:
		template <typename TObject, typename... TArgs>
		static std::shared_ptr<TObject> Create(TArgs&&... args);
	};
}

// --------------------------------------------------------

template <typename TObject, typename... TArgs>
std::shared_ptr<TObject> aiva::utils::ObjectFactory::Create(TArgs&&... args)
{
	return std::shared_ptr<TObject>{new TObject{ std::forward<TArgs>(args)... }};
}
