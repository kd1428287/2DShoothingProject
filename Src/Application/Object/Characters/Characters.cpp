#include "Characters.h"
#include "Application/System/RenderManager/ObjectData.h"

void Characters::Init()
{
}

void Characters::PreUpdate(float dt)
{
}

void Characters::Update(float dt)
{
	Movement(dt);
}

void Characters::Movement(float dt)
{
	objParameter.position += charaParameter.moveDir * dt;
}
