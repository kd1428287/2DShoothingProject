#pragma once

#include "../Items.h"

enum class ThrowState
{
	None,
	Throw,
	Point
};

struct ThrowblesParameter
{
	

	ThrowState state = ThrowState::None;
	float damageRange{};
	int damage;

};

class Throwables :public Items
{
public:
	Throwables() {};
	virtual ~Throwables() override {};

	void Update(float dt)override;

	virtual void Charge(float dt);
	virtual void Throw();

protected:
	ThrowblesParameter thParameter{};
};