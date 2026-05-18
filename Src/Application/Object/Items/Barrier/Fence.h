#pragma once

#include "../Items.h"

class Fence : public Items
{
public:
    Fence() {};
    ~Fence() override {};

    void Update(float dt) override;
    void Init() override;
    void DrawRequest() override;

    void OnCollision(Collider* self, const HitResult& hit) override;

protected:
    int HP = 10;
    int maxHP = 10;

    // --- 被弾演出用 ---
    float hitShakeTimer = 0.0f;     // 揺れ時間のタイマー
    float hitShakeIntensity = 0.0f; // 揺れの強さ
    Math::Vector2 shakeOffset = { 0.0f, 0.0f }; // 描画時のズレ
};