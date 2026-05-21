#include "AudioManager.h"

// KdGameLibのヘッダ (プロジェクトに合わせてパスを確認してください)
// #include "KdSoundEffect.h" 
// #include "KdSoundInstance.h"

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
	Release();
}

void AudioManager::LoadSound(const std::string& name, const std::string& path)
{
	// まだロードされていない場合のみロード
	if (m_soundMap.find(name) == m_soundMap.end())
	{
		auto sound = std::make_shared<KdSoundEffect>();
		if (sound->Load(path))
		{
			m_soundMap[name] = sound;
		}
	}
}

void AudioManager::Init()
{
	//// --- リソース名定数（アクセスしやすいようにpublic定数またはゲッターで公開） ---
	//const std::string BGM_TITLE =	
	//const std::string BGM_GAME =	
	//const std::string SE_DAMAGE =	
	//const std::string SE_HEAL =		
	//const std::string SE_ATTACK =	
	//const std::string SE_EQUIP =	
	//const std::string SE_SHOT =		
	//const std::string SE_BUTTON =	
	//const std::string SE_ALERT =	
	// 使用するすべての音声をロードしておく
	LoadSound("title","Sound/fantasy-ihen-.wav");
	LoadSound("game","Sound/maou_game_dangeon10.wav");
	LoadSound("drum","");
	LoadSound("result","");
	LoadSound("fanfare","");
	LoadSound("exprosion","");
	LoadSound("damage1","");
	LoadSound("damage","Sound/dageki.wav");
	LoadSound("click", "Sound/決定ボタンを押す18.wav");
	LoadSound("hit1", "Sound/小パンチ.wav");
	LoadSound("hit2", "Sound/小キック.wav");
	LoadSound("hit3", "Sound/中パンチ.wav");
	LoadSound("hit4", "Sound/ショット命中.wav");
}	

void AudioManager::Update()
{
	// 再生が終了したSEインスタンスをリストから削除してメモリ解放
	m_seList.remove_if([](std::shared_ptr<KdSoundInstance>& se) {
		return se->IsPlay() == false;
		});
}

void AudioManager::UpdateFade()
{
	// フェード中か確認
	if (m_bgmFade.active)
	{
		if (m_bgmFade.fadeIn)
		{
			// フェードイン処理
			m_bgmFade.currentVol += m_bgmFade.speed;

			// フェードイン終了判定
			if (m_bgmFade.currentVol >= m_bgmFade.targetVol)
			{
				m_bgmFade.currentVol = m_bgmFade.targetVol;
				m_bgmFade.active = false;
				m_bgmFade.end = true;
			}
		}
		else
		{
			// フェードアウト処理
			m_bgmFade.currentVol -= m_bgmFade.speed;

			// フェードアウト終了判定
			if (m_bgmFade.currentVol <= m_bgmFade.targetVol)
			{
				m_bgmFade.currentVol = m_bgmFade.targetVol;
				m_bgmFade.active = false;
				m_bgmFade.end = true;

				// 完了したら停止
				StopBgm();
			}
		}

		if (m_bgmInst)
		{
			// 実際の音量に反映
			m_bgmInst->SetVolume(m_bgmFade.currentVol);
		}
	}

	if (m_bgmFade.end)
	{
		if (m_requestNextBgm)
		{
			FadeInBgm(m_nextBgm, m_fadeInTime, m_nextBgmLoop);
			m_requestNextBgm = false;
		}
		m_bgmFade.end = false;
	}
}

void AudioManager::PlayBgm(const std::string& resName, bool loop)
{
	// 既に何か再生中なら止める
	StopBgm();

	// マップからデータを探す
	if (m_soundMap.find(resName) != m_soundMap.end())
	{
		// 再生用インスタンス生成
		m_bgmInst = m_soundMap[resName]->CreateInstance(false);
		if (m_bgmInst) {
			m_bgmInst->SetVolume(m_bgmVol);
			m_bgmInst->Play(loop);
		}
	}
}

void AudioManager::StopBgm()
{
	if (m_bgmInst)
	{
		m_bgmInst->Stop();
		m_bgmInst = nullptr;
	}
}

void AudioManager::PlaySe(const std::string& resName, bool loop)
{
	if (m_soundMap.find(resName) != m_soundMap.end())
	{
		// SE用インスタンス生成
		auto instance = m_soundMap[resName]->CreateInstance(false);
		if (instance) {
			
			instance->SetVolume(m_seVol);
			if (resName == "bomb")instance->SetVolume(m_seVol * 0.45f);
			instance->Play(loop);

			// 再生リストに追加して、再生中は破棄されないようにする
			m_seList.push_back(instance);
		}
	}
}

void AudioManager::PlaySeNumLimit(const std::string& resName, bool loop)
{
	if (m_soundMap.find(resName) == m_soundMap.end()) return;

	// 今鳴っているSE数
	int playingCount = static_cast<int>(m_seList.size());

	// 1音あたりの音量を計算（√が一番自然）
	float mixRate = 1.0f / std::sqrtf(playingCount + 1.0f);

	float finalVolume = m_seVol * mixRate;

	// 念のため上限
	if (finalVolume > m_seVol)
		finalVolume = m_seVol;

	auto instance = m_soundMap[resName]->CreateInstance(false);
	if (instance)
	{
		instance->SetVolume(finalVolume);
		instance->Play(loop);
		m_seList.push_back(instance);
	}
}

void AudioManager::PlaySePos(const std::string& name,const Math::Vector2& sePos,bool loop)
{
	if (m_soundMap.find(name) == m_soundMap.end()) return;

	auto instance = m_soundMap[name]->CreateInstance(false);
	if (!instance) return;

	// --- 自機基準で距離計算 ---
	Math::Vector2 diff = sePos - m_playerPos;
	float distance = diff.Length();

	const float maxDistance = 500.0f;

	// 遠すぎたら再生しない（軽量化）
	if (distance >= maxDistance) return;

	float volumeRate = 1.0f - (distance / maxDistance);
	if (volumeRate < 0.0f) volumeRate = 0.0f;

	float finalVol = m_seVol * volumeRate;

	instance->SetVolume(finalVol);
	instance->Play(loop);

	m_seList.push_back(instance);
}

void AudioManager::SetPlayerPos(const Math::Vector2& pos)
{
	m_playerPos = pos;
}

void AudioManager::FadeInBgm(const std::string& resName, float fadeTime, bool loop)
{
	m_bgmFade = {};

	StopBgm();

	if (m_soundMap.find(resName) == m_soundMap.end()) return;

	// ★ m_bgmInst に代入する
	m_bgmInst = m_soundMap[resName]->CreateInstance(false);
	if (!m_bgmInst) return;

	m_bgmInst->SetVolume(0.0f);
	m_bgmInst->Play(loop);

	m_bgmFade.active = true;
	m_bgmFade.fadeIn = true;
	m_bgmFade.end = false;

	m_bgmFade.currentVol = 0.0f;
	m_bgmFade.targetVol = m_bgmVol;

	if (fadeTime <= 0.0f)
	{
		m_bgmFade.speed = m_bgmVol;
	}
	else
	{
		m_bgmFade.speed = m_bgmVol / (fadeTime * 60.0f);
	}
}

void AudioManager::FadeOutBgm(float fadeTime)
{
	m_bgmFade = {};

	// フェード状態をON
	m_bgmFade.active = true;
	m_bgmFade.fadeIn = false;
	m_bgmFade.end = false;

	// フェード開始音量
	m_bgmFade.currentVol = GetBgmVolume();

	// フェード終了音量
	m_bgmFade.targetVol = 0.0f;

	// 1フレームあたりの減少量を設定
	if (fadeTime <= 0.0f)
	{
		m_bgmFade.speed = m_bgmFade.currentVol;
	}
	else
	{
		// fadeTime秒で、現在の音量から0へフェードアウトする
		m_bgmFade.speed = m_bgmFade.currentVol / (fadeTime * 60.0f);
	}
}

void AudioManager::FadeOutAndPlayNext(const std::string& nextBgm, float fadeOutTime, float fadeInTime, bool loop)
{
	m_nextBgm = nextBgm;
	m_requestNextBgm = true;
	m_fadeInTime = fadeInTime;
	m_nextBgmLoop = loop;
	FadeOutBgm(fadeOutTime);
}

void AudioManager::PauseBGM()
{
	if (!m_bgmInst->IsPause()) {
		m_bgmInst->Pause();
	}
}

void AudioManager::ResumeBGM()
{
	if(m_bgmInst->IsPause()){
		m_bgmInst->Resume();
	}
}


void AudioManager::Release()
{
	StopBgm();
	m_seList.clear();
	m_soundMap.clear();
}

void AudioManager::SetBgmVolume(float vol)
{
	if (vol < 0.0f) vol = 0.0f;
	if (vol > 1.0f) vol = 1.0f;

	m_bgmVol = vol;

	// 再生中のBGMがあれば即座に反映
	if (m_bgmInst) {
		m_bgmInst->SetVolume(m_bgmVol);
	}
}

void AudioManager::SetSeVolume(float vol)
{
	if (vol < 0.0f) vol = 0.0f;
	if (vol > 1.0f) vol = 1.0f;

	m_seVol = vol;

	// 再生中のすべてのSEにも反映させるならリストを回す
	for (auto& se : m_seList) {
		if (se) se->SetVolume(m_seVol);
	}
}