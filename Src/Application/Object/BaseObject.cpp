#include "BaseObject.h"
#include "Application/System/CollisionManager/CollisionManager.h"
#include "Application/System/CollisionManager/Collider.h"
#include "Application/System/RenderManager/ObjectData.h"
#include "Application/System/RenderManager/RenderManager.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Application/Scene/SceneManager.h"
#include "Application/System/InputManager/InputManager.h"


BaseObject::BaseObject()
{

}

BaseObject::~BaseObject()
{

}

void BaseObject::Init()
{

}

void BaseObject::PreUpdate(float dt)
{

}

void BaseObject::Update(float dt)
{

}

void BaseObject::DrawRequest()
{
    ObjectData data;
    data.tex;

	data.tex = objParameter.tex;
	data.position = objParameter.position;
	data.size = objParameter.size;
	data.scale = objParameter.scale;
	data.rectPosition = objParameter.rectPosition;
	data.angle = objParameter.angle;
	data.priority = objParameter.priority;
	data.footPosition = objParameter.footPosition;
	data.mat = objParameter.mat;
	data.color = objParameter.color;
	data.flashValue = objParameter.flashValue;

	data.target = objParameter.target;
	data.addEffect = objParameter.addEffect;

    RENDERM.Submit(data);
}

void BaseObject::AddCollider(std::unique_ptr<Collider> collider)
{
    CollisionManager::Instance().Register(collider.get());
    colliders_.push_back(std::move(collider));
}

const std::vector<std::unique_ptr<Collider>>&
BaseObject::GetColliders() const
{
    return colliders_;
}
