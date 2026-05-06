#pragma once

#include "../Throwables.h"

class Stone : public Throwables
{
public:
	Stone() {};
	~Stone()override {};

	void Charge(float dt)override;
	void Throw()override;

private:
	float chargeRatio{};
};