#pragma once
#include <vector>
#include <memory>

//class BaseObject;

#include "Application/Object/BaseObject.h"

enum class ScenePaturn
{
	None,
	Title,
	Game,
	Result,
};

class UIManager
{
public:
	static UIManager& Instance()
	{
		static UIManager instance;
		return instance;
	}

	void CreateUI(ScenePaturn type);

	void Update(float dt);
	void DrawRequest();

	void Clear() { uiObjects_.clear(); }

	// ★追加：リザルト画面のスコアカウントアップが完了したかどうかの通知を取得
	bool IsResultScoreDone() const { return isResultScoreDone_; }
	void SetResultScoreDone(bool done) { isResultScoreDone_ = done; }

private:
	UIManager() {};
	~UIManager() {};

	// 作成したUIオブジェクト群を管理するリスト
	std::vector<std::unique_ptr<BaseObject>> uiObjects_;

	// ★追加：リザルトスコアのカウントアップ完了通知用フラグ
	bool isResultScoreDone_ = false;
};