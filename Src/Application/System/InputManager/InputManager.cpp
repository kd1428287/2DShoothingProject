#include "InputManager.h"

#include "Application/main.h"

void InputManager::GetMousePos(POINT* mp)
{
	GetCursorPos(mp);
	ScreenToClient(APP.m_window.GetWndHandle(), mp);

	/*mp->x -= scrWidth / 2;
	mp->y = scrHeight / 2 - mp->y;*/
}
