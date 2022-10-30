#pragma once
#include <pch.h>

#include <aiva/utils_ext/i_object.h>

namespace aiva::utils_ext
{
	template <typename TObjectType>
	struct TObject : private boost::noncopyable, public std::enable_shared_from_this<TObjectType>, public IObject
	{
	// ----------------------------------------------------
	// Main

	public:
		using ThisType = TObjectType;

	protected:
		TObject();

	public:
		~TObject() override;

	// ----------------------------------------------------
	// Construction

	public:
		template <typename... TArgs>
		static std::shared_ptr<TObjectType> New(TArgs&&... args);
	};
}

// --------------------------------------------------------

namespace aiva::utils_ext
{
	template <typename TObjectType>
	TObject<TObjectType>::TObject()
	{

	}

	template <typename TObjectType>
	TObject<TObjectType>::~TObject()
	{

	}

	template <typename TObjectType>
	template <typename... TArgs>
	std::shared_ptr<TObjectType> TObject<TObjectType>::New(TArgs&&... args)
	{
		return std::shared_ptr<TObjectType>{new TObjectType{ std::forward<TArgs>(args)... }};
	}
}
