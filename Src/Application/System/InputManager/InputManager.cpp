#include "InputManager.h"

#include "Application/main.h"

void InputManager::GetMousePos(POINT* mp)
{
	GetCursorPos(mp);
	ScreenToClient(APP.m_window.GetWndHandle(), mp);

	mp->x -= 1280 / 2;
	mp->y = 720 / 2 - mp->y;
}
