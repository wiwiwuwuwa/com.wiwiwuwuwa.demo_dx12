#include <pch.h>
#include <aiva/utils/action_batcher.h>

#include <aiva/utils/asserts.h>

void aiva::utils::ActionBatcher::BeginBatch()
{
	mCounter++;
}

void aiva::utils::ActionBatcher::EndBatch()
{
	aiva::utils::Asserts::CheckBool(mCounter > 0);

	mCounter--;

	if (mCounter != 0)
	{
		return;
	}

	OnBatched()();
}

aiva::utils::EvAction& aiva::utils::ActionBatcher::OnBatched()
{
	return mOnBatched;
}
