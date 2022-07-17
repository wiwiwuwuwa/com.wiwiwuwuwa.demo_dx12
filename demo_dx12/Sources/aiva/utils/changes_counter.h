#pragma once
#include <pch.h>

#include <aiva/utils/ev_action.h>

namespace aiva::utils
{
	struct ChangesCounter final : private boost::noncopyable
	{
	public:
		aiva::utils::EvAction& OnChangesFinished();

		void IncrementChanges();

		void DecrementChanges();

	private:
		aiva::utils::EvAction mOnChangesFinished{};

		int32_t mChangesCounter{};
	};
}
