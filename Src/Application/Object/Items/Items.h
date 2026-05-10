#pragma once

#include "../BaseObject.h"

class Items :public BaseObject
{
public:
	Items() {};
	virtual ~Items()override { BaseObject::~BaseObject(); };
};