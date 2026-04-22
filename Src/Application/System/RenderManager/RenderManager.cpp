#include "RenderManager.h"
#include "RendData.h"

void RenderManager::DrawSprite()
{

}

void RenderManager::Submit(RendData queue)
{
}

void RenderManager::BlackoutRatio(float ratio)
{
}

void RenderManager::BackgroundDraw()
{
}

void RenderManager::BackDraw()
{
    if (backQueue.empty()) return;

    std::stable_sort(backQueue.begin(), backQueue.end(),
        [](const RendData& a, const RendData& b) {
            // priority ‚ªˆÙ‚È‚é‚È‚ç priority —Dæ
            if (a.priority != b.priority) {
                return a.priority < b.priority;
            }
            // priority ‚ª“¯‚¶‚È‚ç footPosition (YÀ•W) ‚Å‘OŒãŠÖŒW‚ğŒˆ‚ß‚é
            return a.footPosition > b.footPosition;
        });

    auto it = std::stable_partition(backQueue.begin(), backQueue.end(),
        [](const RendData& cmd) {
            return !cmd.addEffect; // addEffect ‚ª false ‚Ì‚à‚Ì‚ğ‘O‚ÉW‚ß‚é
        });

    // --- •`‰æÀs ---

    // ’Êí•`‰æ (Alpha)
    for (auto i = backQueue.begin(); i != it; ++i)
    {
        SHADER.m_spriteShader.SetMatrix(i->mat);
        SHADER.m_spriteShader.SetFlashValue(i->flashValue);
        SHADER.m_spriteShader.DrawTex_color(i->tex, Math::Rectangle(i->rectPos.x, i->rectPos.y, i->size.x, i->size.y), i->clr);
    }

    // ‰ÁZ•`‰æ (Add) - ‹«ŠE iterator ˆÈ~‚É true ‚ªW‚Ü‚Á‚Ä‚¢‚é
    if (it != backQueue.end())
    {
        D3D.SetBlendState(BlendMode::Add);
        for (auto i = it; i != backQueue.end(); ++i)
        {
            SHADER.m_spriteShader.SetMatrix(i->mat);
            SHADER.m_spriteShader.SetFlashValue(i->flashValue);
            SHADER.m_spriteShader.DrawTex_color(i->tex, Math::Rectangle(i->rectPos.x, i->rectPos.y, i->size.x, i->size.y), i->clr);
        }
        D3D.SetBlendState(BlendMode::Alpha); // ó‘Ô‚ğ–ß‚·
    }

    SHADER.m_spriteShader.SetFlashValue(0.0f);

    // 3. ƒƒ‚ƒŠ‚ÌÅ“K‰»: clear() ‚Í—e—Ê(capacity)‚ğˆÛ‚·‚é‚½‚ßAŸ‚ÌƒtƒŒ[ƒ€‚Å‚ÌÄŠm•Û‚ğ–h‚°‚Ü‚·
    backQueue.clear();
}

void RenderManager::MiddleDraw()
{
}

void RenderManager::FrontDraw()
{
}

void RenderManager::UIDraw()
{
}
