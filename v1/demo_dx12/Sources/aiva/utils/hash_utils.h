#pragma once
#include <pch.h>

namespace aiva::utils
{
	struct HashUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		HashUtils() = delete;

	// ----------------------------------------------------
	// Utils

	public:
		template <typename T>
		static std::size_t Hash(T const& value);

		template <typename T, typename... TArgs>
		static std::size_t Hash(T const& value, TArgs&&... values);
	};
}

// --------------------------------------------------------

namespace aiva::utils
{
	template <typename T>
	std::size_t HashUtils::Hash(T const& value)
	{
		return boost::hash<T>()(value);
	}

	template <typename T, typename... TArgs>
	std::size_t HashUtils::Hash(T const& value, TArgs&&... values)
	{
		auto hash = std::size_t{};
		boost::hash_combine(hash, Hash(value));
		boost::hash_combine(hash, Hash(std::forward<TArgs>(values)...));
		return hash;
	}
}
