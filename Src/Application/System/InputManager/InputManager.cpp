#include "InputManager.h"

#include "Application/main.h"

Math::Vector2 InputManager::GetMousePos()
{
	POINT* mp;
	GetCursorPos(mp);
	ScreenToClient(APP.m_window.GetWndHandle(), mp);

	mp->x -= 1280 / 2;
	mp->y = 720 / 2 - mp->y;
	Math::Vector2 answer;
	answer = { mp->x,mp->y };
	return answer;
}

float InputManager::GetMouseDir(Math::Vector2 pos)
{
	Math::Vector2 mp;
	mp = INPUT.GetMousePos();

	float dx = mp.x - pos.x;
	float dy = mp.y - pos.y;

	float mouseDir;

	mouseDir = std::atan2(dx, dy) * -1;

	//mouseDis = sqrt(dx * dx + dy * dy);
	return mouseDir;
}
