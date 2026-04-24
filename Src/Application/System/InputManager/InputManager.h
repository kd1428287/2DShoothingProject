#pragma once

class InputManager
{
public:

	static InputManager& Instance()
	{
		static InputManager instance;
		return instance;
	}

	void Update() {
		// 前回の状態を保存
		prevKeys = currentKeys;

		for (int i = 0; i < 256; ++i) {
			currentKeys[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
		}
	}

	void GetMousePos(POINT* mp);

	// キーが「押された瞬間」 (Trigger)
	bool IsTriggered(int key) const {
		return currentKeys[key] && !prevKeys[key];
	}

	// キーが「押され続けている」 (Press)
	bool IsPressed(int key) const {
		return currentKeys[key];
	}

	// キーが「離された瞬間」 (Release)
	bool IsReleased(int key) const {
		return !currentKeys[key] && prevKeys[key];
	}

private:
	InputManager() 
	{
		for (int i = 0; i < 256; ++i) {
			currentKeys[i] = false;
			prevKeys[i] = false;
		}
	};

	~InputManager() {};

	// 256個の仮想キーコードの状態を保存（前回と今回）
	std::array<bool, 256> currentKeys;
	std::array<bool, 256> prevKeys;
};

#define INPUT InputManager::Instance()