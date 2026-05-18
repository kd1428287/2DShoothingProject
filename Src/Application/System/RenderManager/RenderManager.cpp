#include "RenderManager.h"
#include "ObjectData.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include <cstdlib> // rand() 使用のため追加

void RenderManager::Update()
{
    // 画面揺らし（シェイク）の減衰更新
    if (m_shakeIntensity > 0.1f)
    {
        // -1.0f ～ 1.0f のランダムな値に強さを掛けてオフセットを決定
        m_shakeOffset.x = ((rand() % 100) / 50.0f - 1.0f) * m_shakeIntensity;
        m_shakeOffset.y = ((rand() % 100) / 50.0f - 1.0f) * m_shakeIntensity;

        // 毎フレーム揺れを減衰させる
        m_shakeIntensity *= 0.85f;
    }
    else
    {
        m_shakeIntensity = 0.0f;
        m_shakeOffset = { 0.0f, 0.0f };
    }

    // 画面赤フラッシュの減衰更新
    if (m_damageRedAlpha > 0.0f)
    {
        m_damageRedAlpha -= 0.02f; // 徐々に薄くする
        if (m_damageRedAlpha < 0.0f)
        {
            m_damageRedAlpha = 0.0f;
        }
    }
}

void RenderManager::StartDamageReaction(float shakeIntensity, float redFlashAlpha)
{
    m_shakeIntensity = shakeIntensity;
    m_damageRedAlpha = redFlashAlpha;
}

void RenderManager::DrawSprite()
{
    D3D.SetBlendState(BlendMode::NoBlend);

    tmpTex.ClearRenderTarget(Math::Color{ 0,0,0,0 });
    tmpTex.SetRenderTarget();

    D3D.SetBlendState(BlendMode::Alpha);

    BackgroundDraw();
    TreeGeneration();

    DrawQueue(backQueue);
    DrawQueue(middleQueue);
    DrawQueue(frontQueue);

    D3D.SetBackBuffer();

    // シェイクのオフセット(m_shakeOffset)を適用してゲーム画面全体を描画
    SHADER.m_spriteShader.SetMatrix(
        Math::Matrix::CreateScale(tmpScale, tmpScale, 0.0f) *
        Math::Matrix::CreateTranslation(m_shakeOffset.x, m_shakeOffset.y, 0));
    SHADER.m_spriteShader.DrawTex(&tmpTex, Math::Rectangle(0, 0, scrWidth, scrHeight), 1.0f);

    DrawQueue(UIQueue);

    // ダメージフラッシュ（薄い赤色）の描画
    if (m_damageRedAlpha > 0.0f)
    {
        Math::Color redColor = { 1.0f, 0.0f, 0.0f, m_damageRedAlpha };
        SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale({ 1280.0f,720.0f,0.0f }));
        SHADER.m_spriteShader.DrawTex_color(RESOURCE.GetTexture("dot"), { 1,1,1,1 }, redColor);
    }

    // 暗転フェード等の描画
    SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale({ 1280.0f,720.0f,0.0f }));
    SHADER.m_spriteShader.DrawTex_color(RESOURCE.GetTexture("dot"), { 1,1,1,1 }, { 0.0f,0.0f,0.0f,fadeAlpha });
}

void RenderManager::Submit(ObjectData queue)
{
    if (queue.mat == Math::Matrix(1.f, 0, 0, 0,
        0, 1.f, 0, 0,
        0, 0, 1.f, 0,
        0, 0, 0, 1.f))
    {
        queue.mat = Math::Matrix::CreateScale(queue.scale.x, queue.scale.y, 1) *
            Math::Matrix::CreateRotationZ(queue.angle) *
            Math::Matrix::CreateTranslation(queue.position.x, queue.position.y, 0);
    };

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

void RenderManager::Submit(std::shared_ptr<ObjectData> queue)
{
    queue->mat = Math::Matrix::CreateScale(queue->scale.x, queue->scale.y, 1) *
        Math::Matrix::CreateRotationZ(queue->angle) *
        Math::Matrix::CreateTranslation(queue->position.x, queue->position.y, 0);

    //queue->footPosition = queue->position.y - queue->tall;

    switch (queue->target)
    {
    case DrawTarget::back:
        backQueue.push_back(*queue);
        break;
    case DrawTarget::middle:
        middleQueue.push_back(*queue);
        break;
    case DrawTarget::front:
        frontQueue.push_back(*queue);
        break;
    case DrawTarget::UI:
        UIQueue.push_back(*queue);
        break;

    default:
        break;
    }
}

void RenderManager::Fadeout(std::function<void()> onComplete)
{
    fadeAlpha += 0.1f;

    if (fadeAlpha >= 1.0f)
    {
        fadeAlpha = 1.0f;
        onComplete();
    }
}

void RenderManager::Fadein(std::function<void()> onComplete)
{
    fadeAlpha -= 0.1f;

    if (fadeAlpha <= -0.1f)
    {
        fadeAlpha = -0.1f;
        onComplete();
    }
}

RenderManager::RenderManager()
{
    tmpTex.CreateRenderTarget(scrWidth, scrHeight);
    tmpScale = 1.0f;

    backTex = RESOURCE.GetTexture("background");
    treeTex = RESOURCE.GetTexture("tree");

    fadeAlpha = 0.0f;
    fadeColor = { 0.0f,0.0f,0.0f,fadeAlpha };
}

void RenderManager::BackgroundDraw()
{
    Math::Matrix mat = Math::Matrix::CreateScale(0.7f,0.7f,1.0f) *
        Math::Matrix::CreateTranslation(0, 0, 0);
    SHADER.m_spriteShader.SetMatrix(mat);
    SHADER.m_spriteShader.DrawTex_color(backTex, Math::Rectangle(0, 0, 2680, 1600), { 1.0f,1.0f,1.0f,1.0f });
}

void RenderManager::TreeGeneration()
{
    ObjectData data;
    data.tex = treeTex;
    data.size = { 500,500 };
    data.scale = { 0.5f,0.5f };
    

    data.position = { -600.0f,289.0f };
    data.footPosition = data.position.y - 120.0f;
    
    Submit(data);

    data.position = { -630.0f,75.0f };
    data.footPosition = data.position.y - 120.0f;
 
    Submit(data);

    data.position = { -570.0f,-80.0f };
    data.footPosition = data.position.y - 120.0f;
    
    Submit(data);

    data.position = { -610.0f,-273.0f };
    data.footPosition = data.position.y - 120.0f;
  
    Submit(data);

    data.position = { 630.0f,308.0f };
    data.footPosition = data.position.y - 120.0f;
    
    Submit(data);

    data.position = { 613.0f,78.0f };
    data.footPosition = data.position.y - 120.0f;
  
    Submit(data);

    data.position = { 580.0f,-103.0f };
    data.footPosition = data.position.y - 120.0f;
  
    Submit(data);

    data.position = { 620.0f,-303.0f };
    data.footPosition = data.position.y - 120.0f;
  
    Submit(data);
}

void RenderManager::DrawQueue(std::vector<ObjectData>& queue_)
{
    if (queue_.empty()) return;

    std::stable_sort(queue_.begin(), queue_.end(),
        [](const ObjectData& a, const ObjectData& b) {
            // priority が異なるなら priority 優先
            if (a.priority != b.priority) {
                return a.priority < b.priority;
            }
            // priority が同じなら footPosition (Y座標) で前後関係を決める
            return a.footPosition > b.footPosition;
        });

    auto it = std::stable_partition(queue_.begin(), queue_.end(),
        [](const ObjectData& cmd) {
            return !cmd.addEffect; // addEffect が false のものを前に集める
        });

    // --- 描画実行 ---

    // 通常描画 (Alpha)
    for (auto i = queue_.begin(); i != it; ++i)
    {
        SHADER.m_spriteShader.SetMatrix(i->mat);
        SHADER.m_spriteShader.SetFlashValue(i->flashValue);
        SHADER.m_spriteShader.DrawTex_color(i->tex, Math::Rectangle(i->rectPosition.x, i->rectPosition.y, i->size.x, i->size.y), i->color);
    }

    // 加算描画 (Add) - 境界 iterator 以降に true が集まっている
    if (it != queue_.end())
    {
        D3D.SetBlendState(BlendMode::Add);
        for (auto i = it; i != queue_.end(); ++i)
        {
            SHADER.m_spriteShader.SetMatrix(i->mat);
            SHADER.m_spriteShader.SetFlashValue(i->flashValue);
            SHADER.m_spriteShader.DrawTex_color(i->tex, Math::Rectangle(i->rectPosition.x, i->rectPosition.y, i->size.x, i->size.y), i->color);
        }
        D3D.SetBlendState(BlendMode::Alpha); // 状態を戻す
    }

    SHADER.m_spriteShader.SetFlashValue(0.0f);

    queue_.clear();
}