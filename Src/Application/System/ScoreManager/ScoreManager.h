#pragma once

class ScoreManager
{
public:
	static ScoreManager& Instance()
	{
		static ScoreManager instance;
		return instance;
	}

	void AddScore(int num) { score += num; }
	int GetScore() { return score; }

	void Clear() { score = 0; }

private:
	ScoreManager() {};
	~ScoreManager() {};

	int score = 0;

};