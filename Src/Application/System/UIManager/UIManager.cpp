#include "UIManager.h"
#include "Application/Object/BaseObject.h"
#include "Application/System/RenderManager/RenderManager.h"
#include "Application/System/ResourceManager/ResourceManager.h"

// ※実際の環境に合わせて各マネージャーのヘッダーをインクルードしてください
#include "Application/System/ScoreManager/ScoreManager.h"
#include "Application/Object/Characters/Player/PlayerManager.h"
#include "Application/Object/Characters/Player/Player.h"

namespace {

	// ========================================================
	// 1. スコア表示用UIオブジェクト（右揃え）
	// ========================================================
	class ScoreUI : public BaseObject
	{
	public:
		ScoreUI() {}
		~ScoreUI() override {}

		void Init() override
		{
			// 0〜9が横に並んだ数字テクスチャを想定
			objParameter.tex = RESOURCE.GetTexture("number");
			objParameter.target = DrawTarget::UI;
		}

		void DrawRequest() override {
			// スコア取得（ScoreManagerがあれば使用）
			int score = ScoreManager::Instance().GetScore();
			//int score = 12345; // テスト用

			ObjectData data;
			data.tex = objParameter.tex;
			data.target = DrawTarget::UI;
			data.size = { 32.0f, 32.0f }; // 1コマのピクセルサイズ
			data.scale = { 2.0f, 2.0f };

			// 右上の基準位置
			Math::Vector2 currentPos = { 600.0f, 320.0f };
			float spacing = 26.0f * data.scale.x; // 文字の間隔

			if (score == 0) {
				data.position = currentPos;
				data.rectPosition = { 9 * 32.0f, 0.0f }; // 0は一番右（9番目）
				RENDERM.Submit(data);
				return;
			}

			// 下一桁から順に描画（右揃え）
			while (score > 0) {
				int digit = score % 10;
				score /= 10;

				// ★画像が「1 2 3 4 5 6 7 8 9 0」の場合のロジック
				// 1~9なら(n-1)、0なら9番目のインデックスを使用
				int drawIdx = (digit == 0) ? 9 : (digit - 1);

				data.position = currentPos;
				data.rectPosition = { drawIdx * 32.0f, 0.0f };
				RENDERM.Submit(data);

				currentPos.x -= spacing; // 左へずらす
			}
		}
	};

	// ========================================================
	// 2. 残り耐久力表示用UIオブジェクト（左揃え）
	// ========================================================
	class HpUI : public BaseObject
	{
	public:
		HpUI() {}
		~HpUI() override {}

		void Init() override
		{
			objParameter.tex = RESOURCE.GetTexture("player"); // 耐久力用アイコン
			objParameter.target = DrawTarget::UI;
		}

		void DrawRequest() override
		{
			// ※実際のプレイヤー管理に合わせて残り耐久力を取得してください
			int currentHP = PlayerManager::Instance().GetPlayer()->GetHP();
			//int currentHP = 3; // 仮の値

			ObjectData data;
			data.tex = objParameter.tex;
			data.target = DrawTarget::UI;
			data.size = { 48.0f, 48.0f };
			data.scale = { 1.0f, 1.0f };
			data.color = { 1.0f, 1.0f, 1.0f, 1.0f };

			// 左上の基準座標（一番左のアイコンの位置）
			Math::Vector2 startPos = { -580.0f, 320.0f };
			float iconSpacing = 50.0f; // アイコン同士の並び間隔

			// 左揃えで右に向かってアイコンを並べて描画
			for (int i = 0; i < currentHP; ++i)
			{
				data.position = { startPos.x + (i * iconSpacing), startPos.y };
				data.rectPosition = { 0.0f, 0.0f };
				RENDERM.Submit(data);
			}
		}
	};

	// ========================================================
	// 3. アイテムスロット表示用UIオブジェクト
	// ========================================================
	class ItemSlotUI : public BaseObject
	{
	public:
		ItemSlotUI() {}
		~ItemSlotUI() override {}

		void Init() override
		{
			objParameter.target = DrawTarget::UI;
		}

		void DrawRequest() override
		{
			KdTexture* boxTex = RESOURCE.GetTexture("box");
			KdTexture* stoneTex = RESOURCE.GetTexture("stone");
			KdTexture* bombTex = RESOURCE.GetTexture("bomb");

			ObjectData data;
			data.target = DrawTarget::UI;
			data.scale = { 1.0f, 1.0f };
			data.color = { 1.0f, 1.0f, 1.0f, 1.0f };

			// --- スロット1（Stone用） ---
			Math::Vector2 slot1Pos = { 150.0f, -300.0f };

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
			// ボックスが重ならないようX座標を適度にずらして配置
			Math::Vector2 slot2Pos = { 260.0f, -300.0f };

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
		}
	};

	// ========================================================
	// 4. リザルト画面用スコア表示UI（中央・カウントアップ演出）
	// ========================================================
	class ResultScoreUI : public BaseObject
	{
	public:
		ResultScoreUI() {}
		~ResultScoreUI() override {}

		void Init() override
		{
			objParameter.tex = RESOURCE.GetTexture("number");
			objParameter.target = DrawTarget::UI;

			// 最終的な目標スコアを取得
			targetScore_ = ScoreManager::Instance().GetScore();
			currentDisplayScore_ = 0;

			timer_ = 0.0f;
			duration_ = 2.0f; // 演出時間（2秒でカウントアップ完了）
			isDone_ = false;

			// ★カウントアップ終了までは少し小さめのスケールで開始
			currentScale_ = 3.0f;
		}

		void Update(float dt) override
		{
			if (!isDone_)
			{
				timer_ += dt;
				if (timer_ >= duration_)
				{
					timer_ = duration_;
					currentDisplayScore_ = targetScore_;
					isDone_ = true;

					// ★カウントアップ完了時にシーンへ通知を出せるようフラグをONにする
					UIManager::Instance().SetResultScoreDone(true);
				}
				else
				{
					// イージングアウト（徐々に減速して止まる演出）
					float t = timer_ / duration_;
					float easeOut = t * (2.0f - t);
					currentDisplayScore_ = static_cast<int>(targetScore_ * easeOut);
				}
			}
			else
			{
				// ★カウントアップが終わったときに大きく表示させる処理（滑らかに拡大）
				// 即座に大きくしたい場合は単に currentScale_ = 5.0f; 等にしても構いません
				float targetScale = 5.0f;
				if (currentScale_ < targetScale)
				{
					currentScale_ += dt * 8.0f; // 演出として素早く拡大させる
					if (currentScale_ > targetScale)
					{
						currentScale_ = targetScale;
					}
				}
			}
		}

		void DrawRequest() override
		{
			int score = currentDisplayScore_;

			ObjectData data;
			data.tex = objParameter.tex;
			data.target = DrawTarget::UI;
			data.size = { 32.0f, 32.0f }; // 1コマのピクセルサイズ

			// ★状態に応じて変動するスケールを適用
			data.scale = { currentScale_, currentScale_ };

			float spacing = 26.0f * data.scale.x; // 文字の間隔

			// 桁数を計算して、画面中心に表示されるように調整
			int tempScore = score;
			int digits = 0;
			if (tempScore == 0)
			{
				digits = 1;
			}
			else
			{
				while (tempScore > 0)
				{
					digits++;
					tempScore /= 10;
				}
			}

			// 全体の横幅の真ん中が画面中央(0, 0)に来るよう、下一桁（一番右）のX座標を算出
			Math::Vector2 currentPos = { ((digits - 1) * spacing) / 2.0f, 0.0f };

			if (score == 0)
			{
				data.position = currentPos;
				data.rectPosition = { 9 * 32.0f, 0.0f }; // 0は一番右（9番目）
				RENDERM.Submit(data);
				return;
			}

			// 下一桁から順に描画（右から左へ配置）
			while (score > 0)
			{
				int digit = score % 10;
				score /= 10;

				int drawIdx = (digit == 0) ? 9 : (digit - 1);

				data.position = currentPos;
				data.rectPosition = { drawIdx * 32.0f, 0.0f };
				RENDERM.Submit(data);

				currentPos.x -= spacing; // 左へずらす
			}
		}

	private:
		int targetScore_ = 0;
		int currentDisplayScore_ = 0;
		float timer_ = 0.0f;
		float duration_ = 2.0f;
		bool isDone_ = false;

		// ★現在の表示スケールを保持する変数
		float currentScale_ = 3.0f;
	};

	// ========================================================
	// 5. タイトル画面表示用UIオブジェクト
	// ========================================================
	class TitleUI : public BaseObject
	{
	public:
		TitleUI() {}
		~TitleUI() override {}

		void Init() override
		{
			logoTex_ = RESOURCE.GetTexture("logo");
			tuiTex_ = RESOURCE.GetTexture("titleUI");
			objParameter.target = DrawTarget::UI;

			// ★変数の初期化
			alpha_ = 1.0f;
			timer_ = 0.0f;
		}

		// ★追加：毎フレームの更新処理で透明度を計算
		void Update(float dt) override
		{
			// タイマーを進める（掛ける数値を変えると点滅スピードが変わります）
			timer_ += dt * 2.0f;

			// sin関数を使って 0.2f ～ 1.0f の間を滑らかに往復させる
			// （完全に消えず、少しだけ見える状態から不透明に戻るループです）
			// ※完全に消したい場合は alpha_ = 0.5f + 0.5f * sinf(timer_); にしてください
			alpha_ = 0.6f + 0.4f * sinf(timer_);
		}

		void DrawRequest() override
		{
			ObjectData data;
			data.target = DrawTarget::UI;

			// 1. ロゴの描画（常に不透明）
			if (logoTex_)
			{
				data.color = { 1.0f, 1.0f, 1.0f, 1.0f };
				data.tex = logoTex_;
				data.size = { 600.0f, 100.0f };
				data.scale = { 2.0f, 2.0f };
				data.position = { 0.0f, 100.0f };
				data.rectPosition = { 0.0f, 0.0f };
				RENDERM.Submit(data);
			}

			// 2. タイトルUIの描画（★計算したアルファ値を適用して点滅させる）
			if (tuiTex_)
			{
				data.color = { 1.0f, 1.0f, 1.0f, alpha_ }; // ★ここを変更
				data.tex = tuiTex_;
				data.size = { 507.0f, 91.0f };
				data.scale = { 1.0f, 1.0f };
				data.position = { 0.0f, -200.0f };
				data.rectPosition = { 0.0f, 0.0f };
				RENDERM.Submit(data);
			}
		}

	private:
		KdTexture* logoTex_ = nullptr;
		KdTexture* tuiTex_ = nullptr;

		// ★追加：点滅制御用の変数
		float alpha_ = 1.0f;
		float timer_ = 0.0f;
	};

} // namespace


// ============================================================================
// UIManager の実装部
// ============================================================================

void UIManager::CreateUI(ScenePaturn type)
{
	// 前のシーンのUI群を安全にクリア
	uiObjects_.clear();

	// ★UI作成時に完了通知フラグを初期化
	isResultScoreDone_ = false;

	switch (type)
	{
	case ScenePaturn::Game:
	{
		// 1. スコア表示の作成と登録
		auto scoreUI = std::make_unique<ScoreUI>();
		scoreUI->Init();
		uiObjects_.push_back(std::move(scoreUI));

		// 2. 耐久力表示の作成と登録
		auto hpUI = std::make_unique<HpUI>();
		hpUI->Init();
		uiObjects_.push_back(std::move(hpUI));

		// 3. アイテムスロット表示の作成と登録
		auto itemSlotUI = std::make_unique<ItemSlotUI>();
		itemSlotUI->Init();
		uiObjects_.push_back(std::move(itemSlotUI));

		break;
	}
	case ScenePaturn::Result:
	{
		// リザルト画面用スコア表示の作成と登録（中央カウントアップ）
		auto resultScoreUI = std::make_unique<ResultScoreUI>();
		resultScoreUI->Init();
		uiObjects_.push_back(std::move(resultScoreUI));

		break;
	}
	case ScenePaturn::Title: // ★ここに処理を追加
	{
		auto titleUI = std::make_unique<TitleUI>();
		titleUI->Init();
		uiObjects_.push_back(std::move(titleUI));
		break;
	}
	default:
		break;
	}
}

void UIManager::Update(float dt)
{
	for (auto& ui : uiObjects_)
	{
		ui->PreUpdate(dt);
		ui->Update(dt);
	}
}

void UIManager::DrawRequest()
{
	for (auto& ui : uiObjects_)
	{
		ui->DrawRequest();
	}
}