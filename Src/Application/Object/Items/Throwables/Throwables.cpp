#include "Throwables.h"

void Throwables::Update(float dt)
{
	switch (thParameter.state)
	{
	case ThrowState::None:
		break;
	case ThrowState::Throw:
		break;
	case ThrowState::Point:
		break;
	default:
		break;
	}
}

void Throwables::Charge(float dt)
{
}


void Throwables::Throw()
{
}
