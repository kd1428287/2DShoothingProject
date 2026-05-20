#pragma once
#include <map>
#include <list>
#include <string>
#include <memory>

// KdGameLibのクラス前方宣言
class KdSoundEffect;
class KdSoundInstance;

class AudioManager
{
public:
	// シングルトンパターン
	static AudioManager& Instance()
	{
		static AudioManager instance;
		return instance;
	}

	// 初期化（サウンドデータのロード）
	void Init();

	// 更新（再生終了したSEのメモリ解放）
	void Update();

	void UpdateFade();

	// BGM再生
	void PlayBgm(const std::string& resName, bool loop = true);
	// BGM停止
	void StopBgm();

	// SE再生
	void PlaySe(const std::string& resName, bool loop = false);

	// 同時再生時減衰用Se再生
	void PlaySeNumLimit(const std::string& resName, bool loop = false);

	// 距離減衰用Se再生
	void PlaySePos(const std::string& name, const Math::Vector2& sePos, bool loop = false);

	// 自機位置把握用
	void SetPlayerPos(const Math::Vector2& pos);

	// Bgmのフェードイン
	void FadeInBgm(const std::string& resName, float fadeTime, bool loop = false);

	// Bgmのフェードアウト
	void FadeOutBgm(float fadeTime);

	void FadeOutAndPlayNext(const std::string& nextBgm, float fadeOutTime, float fadeInTime, bool loop);

	void PauseBGM();
	void ResumeBGM();

	// 解放
	void Release();

	void SetBgmVolume(float vol);
	void SetSeVolume(float vol);
	float GetBgmVolume() const { return m_bgmVol; }
	float GetSeVolume() const { return m_seVol; }

private:
	AudioManager();
	~AudioManager();

	// サウンドデータを管理するマップ (ファイル名 -> 音データ)
	std::map<std::string, std::shared_ptr<KdSoundEffect>> m_soundMap;

	// 現在再生中のBGM
	std::shared_ptr<KdSoundInstance> m_bgmInst;

	// 再生中のSEリスト (再生が終わるまでインスタンスを保持する必要があるため)
	std::list<std::shared_ptr<KdSoundInstance>> m_seList;

	// 音量設定
	float m_bgmVol = 0.3f;
	float m_seVol = 0.5f;

	struct FadeParam
	{
		bool active = false;
		bool fadeIn = false;
		std::string type;
		float currentVol = 0.0f;
		float targetVol = 0.0f;
		float speed = 0.0f;

		bool end = false;
	};

	FadeParam m_bgmFade = {};
	std::string m_nextBgm = {};
	bool m_requestNextBgm = false;
	float m_fadeInTime = 0.0f;
	bool m_nextBgmLoop = false;
	Math::Vector2 m_listenerPos{};
	Math::Vector2 m_playerPos{};

	// ロード用ヘルパー関数
	void LoadSound(const std::string& name, const std::string& path);
};

// マクロ定義：どこからでも AUDIO.PlaySe(...) で呼べるようにする
#define AUDIOM AudioManager::Instance()