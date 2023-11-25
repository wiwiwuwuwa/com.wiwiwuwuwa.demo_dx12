#pragma once
#include <pch.h>

#include <aiva/utils/ev_action.h>

namespace aiva::utils
{
	struct ActionBatcher final : private boost::noncopyable
	{
	// ----------------------------------------------------
	// Actions

	public:
		void BeginBatch();

		void EndBatch();

	private:
		std::uint64_t mCounter{};

	// ----------------------------------------------------
	// Events

	public:
		EvAction& OnBeginBatch();

		EvAction& OnEndBatch();

	private:
		EvAction mOnBeginBatch{};

		EvAction mOnEndBatch{};
	};
}
