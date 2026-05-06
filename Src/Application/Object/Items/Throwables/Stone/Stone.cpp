#include "Stone.h"
#include "Application/System/InputManager/InputManager.h"

void Stone::Charge(float dt)
{
    if (INPUT.IsTriggered(VK_LBUTTON))
    {
        // チャージ初期化
        chargeRatio = 1.0f;
    }
    else if (INPUT.IsPressed(VK_LBUTTON))
    {
        // チャージ中

        if (chargeRatio >= 600.0f)
        {
            chargeRatio = 600.0f;
            return;
        }

        //縦画面の過半
        chargeRatio += 600.0f * dt;
    }
    else if (INPUT.IsReleased(VK_LBUTTON))
    {
        // チャージ終わり
        Throw();
        chargeRatio = 1.0f;
    }
}

void Stone::Throw()
{
}
