#include <pch.h>
#include <aiva/utils/i_object_changeable.h>

aiva::utils::IObjectChangeable::IObjectChangeable()
{

}

aiva::utils::IObjectChangeable::~IObjectChangeable()
{

}

aiva::utils::EvAction& aiva::utils::IObjectChangeable::OnChanged()
{
	return mOnChanged;
}
