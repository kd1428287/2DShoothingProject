#include "Goblin.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Application/System/CollisionManager/Collider.h"

Goblin::~Goblin()
{
	BaseEnemy::~BaseEnemy();
}

void Goblin::SetParameter()
{
	// ゴブリン固有のパラメータ設定
	charaParameter.HP = 1;             // ゴブリンはHP1とする
	charaParameter.moveDir = { 0, -1 }; // 下方向に進む
	charaParameter.moveSpeed = 120.0f;

	float x = rand() % 1200 - 600;
	//float x = 0;
	objParameter.position = { x, 430.0f };

	Init();
}

void Goblin::Init()
{
	// 共通の初期化（コライダー生成やステート初期化）を先に呼ぶ
	BaseEnemy::Init();

	// 画像とサイズをゴブリン専用のものに上書き
	objParameter.tex = RESOURCE.GetTexture("troll");
	objParameter.scale = { 0.2f, 0.2f };
	objParameter.size = { 500.0f, 500.0f };

	// コライダーのサイズもゴブリンの画像サイズに合わせる
	if (!colliders_.empty()) {
		colliders_[0]->SetCircle(45.0f);
	}
}

void Goblin::WalkUpdate(float dt)
{
	// 1. 前進移動（基本の移動速度による更新）
	Movement(dt);

	// 2. ゴブリン特有の動き（提示ファイルにあった左右への揺れ処理を追加）
	if ((shapeCount += dt) > 1.0f)
	{
		shapeCount = 0.0f;
		objParameter.position.x += shape;
		shape *= -1;
	}
}