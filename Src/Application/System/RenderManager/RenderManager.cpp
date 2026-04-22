#include "RenderManager.h"
#include "RendData.h"

void RenderManager::DrawSprite()
{
    BackgroundDraw();

    DrawQueue(backQueue);
    DrawQueue(middleQueue);
    DrawQueue(frontQueue);
    DrawQueue(UIQueue);


}

void RenderManager::Submit(RendData queue)
{
    switch (queue.target)
    {
    case DrawTarget::back:
        backQueue.push_back(queue);
        break;
    case DrawTarget::middle:
        middleQueue.push_back(queue);
        break;
    case DrawTarget::front:
        frontQueue.push_back(queue);
        break;
    case DrawTarget::UI:
        UIQueue.push_back(queue);
        break;

    default:
        break;
    }
}

void RenderManager::BlackoutRatio(float ratio)
{
}

void RenderManager::BackgroundDraw()
{
}

void RenderManager::DrawQueue(std::vector<RendData> queue_)
{
    if (queue_.empty()) return;

    std::stable_sort(queue_.begin(), queue_.end(),
        [](const RendData& a, const RendData& b) {
            // priority ‚ªˆÙ‚È‚é‚È‚ç priority —Dæ
            if (a.priority != b.priority) {
                return a.priority < b.priority;
            }
            // priority ‚ª“¯‚¶‚È‚ç footPosition (YÀ•W) ‚Å‘OŒãŠÖŒW‚ğŒˆ‚ß‚é
            return a.footPosition > b.footPosition;
        });

    auto it = std::stable_partition(queue_.begin(), queue_.end(),
        [](const RendData& cmd) {
            return !cmd.addEffect; // addEffect ‚ª false ‚Ì‚à‚Ì‚ğ‘O‚ÉW‚ß‚é
        });

    // --- •`‰æÀs ---

    // ’Êí•`‰æ (Alpha)
    for (auto i = queue_.begin(); i != it; ++i)
    {
        SHADER.m_spriteShader.SetMatrix(i->mat);
        SHADER.m_spriteShader.SetFlashValue(i->flashValue);
        SHADER.m_spriteShader.DrawTex_color(i->tex, Math::Rectangle(i->rectPosition.x, i->rectPosition.y, i->size.x, i->size.y), i->color);
    }

    // ‰ÁZ•`‰æ (Add) - ‹«ŠE iterator ˆÈ~‚É true ‚ªW‚Ü‚Á‚Ä‚¢‚é
    if (it != queue_.end())
    {
        D3D.SetBlendState(BlendMode::Add);
        for (auto i = it; i != queue_.end(); ++i)
        {
            SHADER.m_spriteShader.SetMatrix(i->mat);
            SHADER.m_spriteShader.SetFlashValue(i->flashValue);
            SHADER.m_spriteShader.DrawTex_color(i->tex, Math::Rectangle(i->rectPosition.x, i->rectPosition.y, i->size.x, i->size.y), i->color);
        }
        D3D.SetBlendState(BlendMode::Alpha); // ó‘Ô‚ğ–ß‚·
    }

    SHADER.m_spriteShader.SetFlashValue(0.0f);

    // 3. ƒƒ‚ƒŠ‚ÌÅ“K‰»: clear() ‚Í—e—Ê(capacity)‚ğˆÛ‚·‚é‚½‚ßAŸ‚ÌƒtƒŒ[ƒ€‚Å‚ÌÄŠm•Û‚ğ–h‚°‚Ü‚·
    queue_.clear();
}
