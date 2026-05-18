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
    //objParameter.footPosition = -9999.9f;
}

BaseObject::~BaseObject()
{
    for (auto& col : colliders_)
    {
        CollisionManager::Instance().Unregister(col.get());
    }
    colliders_.clear();
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

    data.position = { objParameter.position.x,objParameter.position.y + objParameter.floating };
    data.footPosition = objParameter.position.y - objParameter.tall + objParameter.floating;

	data.size = objParameter.size;
	data.scale = objParameter.scale;
	data.rectPosition = objParameter.rectPosition;
	data.angle = objParameter.angle;
	data.priority = objParameter.priority;
	data.mat = objParameter.mat;
	data.color = objParameter.color;
	data.flashValue = objParameter.flashValue;

	data.target = objParameter.target;
	data.addEffect = objParameter.addEffect;

    RENDERM.Submit(data);
}

void BaseObject::PositionSync()
{
	for (auto &col : colliders_)
	{
		col->SetPosition(objParameter.position);
	}
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

void BaseObject::DrawShadow(const ObjectParameter& param)
{
    ObjectData shadowData;

    shadowData.tex = RESOURCE.GetTexture("shadow");
    shadowData.size = { 64.0f,64.0f };
    float scaleX = param.visualSize.x / shadowData.size.x;
    float scaleY = scaleX * 0.35f;
    float attenuate = 1.0f - (param.floating / 150.0f);
    if (attenuate < 0.2f) attenuate = 0.2f; // 消えきらないように最低値を保証
    shadowData.scale = { scaleX * attenuate,scaleY * attenuate };
    float posY = param.position.y - param.tall - param.floating;
    shadowData.position = { param.position.x, posY};

    shadowData.color = { 0.0f, 0.0f,0.0f, 0.4f * attenuate };
    shadowData.addEffect = false;

    // 行列の初期化（Submit側で自動計算させるためのトリガー値にする）
    shadowData.mat = Math::Matrix(1.f, 0, 0, 0,
        0, 1.f, 0, 0,
        0, 0, 1.f, 0,
        0, 0, 0, 1.f);

    shadowData.priority = objParameter.priority - 0.05f;

    // RenderManager へ送信
    RENDERM.Submit(shadowData);
}
