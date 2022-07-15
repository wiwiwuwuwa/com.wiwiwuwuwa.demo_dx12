#include <pch.h>
#include <aiva/utils/changes_counter.h>

#include <aiva/utils/asserts.h>

aiva::utils::EvAction& aiva::utils::ChangesCounter::OnChangesFinished()
{
	return mOnChangesFinished;
}

void aiva::utils::ChangesCounter::IncrementChanges()
{
	mChangesCounter++;
}

void aiva::utils::ChangesCounter::DecrementChanges()
{
	mChangesCounter--;

	aiva::utils::Asserts::CheckBool(mChangesCounter >= 0);

	if (mChangesCounter == 0)
	{
		OnChangesFinished()();
	}
}
