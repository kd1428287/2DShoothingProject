#pragma once
#include "../Enemy.h" // BaseEnemyを継承するため

class Goblin : public BaseEnemy
{
public:
	Goblin() {};
	~Goblin() override;

	void SetParameter() override;
	void Init() override;

protected:
	// ゴブリン固有の移動ロジックのみをオーバーライド
	void WalkUpdate(float dt) override;

private:
	float shapeCount = 0.0f;
	float shape = 2.0f; // 元のGoblin.cppにあった揺れ幅変数
};