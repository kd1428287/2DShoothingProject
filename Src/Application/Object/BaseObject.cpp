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
    objParameter.footPosition = -9999.9f;
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
	data.position = objParameter.position;
	data.size = objParameter.size;
	data.scale = objParameter.scale;
	data.rectPosition = objParameter.rectPosition;
	data.angle = objParameter.angle;
	data.priority = objParameter.priority;
    if (objParameter.footPosition == -9999.9f)objParameter.footPosition = objParameter.position.y;
	data.footPosition = objParameter.footPosition;
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

void BaseObject::DrawShadow(const Math::Vector2& groundPos, float shadowHeightOffset)
{
    // 影用のオブジェクトデータを構築
    ObjectData shadowData;
    
    // 影のテクスチャを取得（ ResourceManager で "shadow" と命名して読み込んでいると仮定）
    shadowData.tex = RESOURCE.GetTexture("shadow"); 

    // 1. 本体の実際の描画サイズ（幅）を算出
    // scaleがマイナス（左右反転など）のケースも考慮して絶対値を取る
    float realWidth = objParameter.size.x * std::abs(objParameter.scale.x);
    
    // 影の基本サイズを決定（例：テクスチャサイズが64x64だとして、横幅は本体の実サイズに合わせ、縦幅はその1/3程度に潰す）
    shadowData.size = { 64.0f, 64.0f };
    
    // 実サイズに対するスケール比率を計算
    float shadowScaleX = realWidth / 64.0f;
    float shadowScaleY = shadowScaleX * 0.35f; // 見下ろし視点に合わせて楕円形に潰す

    // 2. 浮遊（高さ）による影の拡縮と薄れ表現
    // 現在の描画Y座標と、地面Y座標の差分が「現在浮いている高さ」
    float currentHeight = objParameter.position.y - groundPos.y;
    if (currentHeight < 0.0f) currentHeight = 0.0f;

    // 高く浮くほど影を少し小さく、かつ透明にする減衰係数（例: 100px浮くと半減）
    float attenuate = 1.0f - (currentHeight / 100.0f);
    if (attenuate < 0.2f) attenuate = 0.2f; // 消えきらないように最低値を保証

    shadowData.scale = { shadowScaleX * attenuate, shadowScaleY * attenuate };

    // 3. 描画位置の設定
    // 本体のX座標と、渡された地面のY座標を基準にする
    shadowData.position = { objParameter.position.x, groundPos.y + shadowHeightOffset };
    shadowData.rectPosition = { 0.0f, 0.0f };

    // 4. ソート順と色の設定
    // 本体と同じターゲット(middle等)に出す
    shadowData.target = objParameter.target;
    
    // ★最重要：本体より奥に描画させるための設定
    // RenderManagerのソート仕様「 priority が異なるなら priority 優先（小さい方が先＝奥に描画）」を利用する
    // 本体の priority から -0.1f して、確実に本体の背面に回す
    shadowData.priority = objParameter.priority - 0.1f;
    
    // footPosition は地面位置を基準にする
    shadowData.footPosition = groundPos.y;

    // 色・透明度（アルファ値も減衰係数をかけて薄くする）
    shadowData.color = { 0.0f, 0.0f,0.0f, 0.6f * attenuate };
    shadowData.addEffect = false;

    // 行列の初期化（Submit側で自動計算させるためのトリガー値にする）
    shadowData.mat = Math::Matrix(1.f, 0, 0, 0,
                                  0, 1.f, 0, 0,
                                  0, 0, 1.f, 0,
                                  0, 0, 0, 1.f);

    // RenderManager へ送信
    RENDERM.Submit(shadowData);
}