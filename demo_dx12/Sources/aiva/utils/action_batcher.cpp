#include <pch.h>
#include <aiva/utils/action_batcher.h>

#include <aiva/utils/asserts.h>

void aiva::utils::ActionBatcher::BeginBatch()
{
	aiva::utils::Asserts::CheckBool(mCounter >= 0);

	if (mCounter == 0)
	{
		OnBeginBatch()();
	}

	mCounter++;
}

void aiva::utils::ActionBatcher::EndBatch()
{
	aiva::utils::Asserts::CheckBool(mCounter >= 1);

	mCounter--;

	if (mCounter == 0)
	{
		OnEndBatch()();
	}
}

aiva::utils::EvAction& aiva::utils::ActionBatcher::OnBeginBatch()
{
	return mOnBeginBatch;
}

aiva::utils::EvAction& aiva::utils::ActionBatcher::OnEndBatch()
{
	return mOnEndBatch;
}
