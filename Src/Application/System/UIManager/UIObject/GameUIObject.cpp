#include "GameUIObject.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Application/System/InputManager/InputManager.h"
#include "Application/System/RenderManager/RenderManager.h"
#include "Application/Object/ObjectManager.h"
#include "Application/Object/Items/ItemManager.h"
#include "Application/Object/Characters/Player/PlayerManager.h"
#include "Application/Object/Characters/Player/Player.h"

void ItemSlotUI::Init()
{
	objParameter.target = DrawTarget::UI;
	boxTex = RESOURCE.GetTexture("box");
	boxHiTex = RESOURCE.GetTexture("box_Hi");
	stoneTex = RESOURCE.GetTexture("stone");
	bombTex = RESOURCE.GetTexture("bomb");
	MouseTex = RESOURCE.GetTexture("mouse");
	RTex = RESOURCE.GetTexture("R");
	LTex = RESOURCE.GetTexture("L");
	wallTex = RESOURCE.GetTexture("wall");

	displayCount = 3.0f;
}

void ItemSlotUI::Update(float dt)
{
	if (displayCount > 0) {
		displayCount -= dt;
	}

	if (INPUT.IsPressed(VK_RBUTTON))
	{
		displayCount = 1.0f;
	}
}

void ItemSlotUI::DrawRequest()
{
	ObjectData data;
	data.target = DrawTarget::UI;
	data.scale = { 0.5f,0.5f };
	data.color = { 1.0f, 1.0f, 1.0f, 1.0f };

	// --- スロット1（Stone用） ---
	Math::Vector2 mousePos = INPUT.GetMousePos();
	Math::Vector2 slot1Pos = mousePos + Math::Vector2(50.0f, 50.0f);
	Math::Vector2 slot2Pos = mousePos + Math::Vector2(100.0f, 50.0f);

	if (displayCount > 0)
	{
		data.color.w = displayCount;
		// 1. 背景のボックスを描画 (96x96)
		data.tex = boxTex;
		data.position = slot1Pos;
		data.size = { 96.0f, 96.0f };
		data.rectPosition = { 0.0f, 0.0f };
		RENDERM.Submit(data);

		// 2. 中身のStoneを描画 (ボックスに収まるよう64x64に縮小)
		// ※安定ソート仕様により、後からSubmitしたものが上に重なります
		data.tex = stoneTex;
		data.position = slot1Pos;
		data.size = { 64.0f, 64.0f };
		RENDERM.Submit(data);


		// --- スロット2（Bomb用） ---

		// 1. 背景のボックスを描画
		data.tex = boxTex;
		data.position = slot2Pos;
		data.size = { 96.0f, 96.0f };
		RENDERM.Submit(data);

		// 2. 中身のBombを描画
		data.tex = bombTex;
		data.position = slot2Pos;
		data.size = { 64.0f, 64.0f };
		RENDERM.Submit(data);

		data.tex = boxHiTex;
		data.size = { 108.0f,108.0f };
		data.priority = -0.5f;
		switch (PlayerManager::Instance().GetPlayer()->NowItem())
		{
		case ItemType::Stone:
			data.position = slot1Pos;
			break;
		case ItemType::Bomb:
			data.position = slot2Pos;
			break;
		default:
			break;
		}
		RENDERM.Submit(data);
	}

	switch (PlayerManager::Instance().GetPlayer()->NowItem())
	{
	case ItemType::Stone:
		data.tex = stoneTex;
		break;
	case ItemType::Bomb:
		data.tex = bombTex;
		break;
	default:
		break;
	}
	data.position = INPUT.GetMousePos();
	data.size = { 64.0f, 64.0f };
	data.scale = { 1.0f,1.0f };
	data.color.w = 0.7f;

	RENDERM.Submit(data);

	data.tex = MouseTex;
	data.size = { 64.0f,64.0f };
	data.color.w = 1.0f;
	data.position = { -600.0f,-250.0f };

	RENDERM.Submit(data);

	data.rectPosition = { 64.0f,0.0f };
	data.position.y -= 64.0f;

	RENDERM.Submit(data);

	data.tex = LTex;
	data.rectPosition = { 0.0f,0.0f };
	data.scale = { 0.8f,0.8f };
	data.size = { 96.0f,96.0f };
	data.position = { -540.0f,-250.0f };

	RENDERM.Submit(data);

	data.tex = RTex;
	data.position.y -= 64.0f;

	RENDERM.Submit(data);

	data.tex = wallTex;
	data.position = { 0.0f,-480.0f};
	data.size = { 964,1024 };
	data.scale = { 1.0f,1.0f };
	data.footPosition = -500.0f;
	data.target = DrawTarget::middle;
	data.priority = 0;

	RENDERM.Submit(data);
}