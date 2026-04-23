#include "ResourceManager.h"

ResourceManager& ResourceManager::Instance()
{
    static ResourceManager instance;
    return instance;
}

void ResourceManager::LoadTexture(const std::string& key, const std::string& filePath)
{
    if (textures_.count(key))return;    //すでにそのキーで情報が登録されているならreturn

    auto tex = std::make_unique<KdTexture>();   //KdTexture型のユニークポインタ

    // Load の失敗も検知する
    if (!tex->Load(filePath))
    {
        assert(false && "Texture load failed");
        return;
    }

    textures_.emplace(key, std::move(tex)); //mapコンテナに登録
}

KdTexture* ResourceManager::GetTexture(const std::string& key) const
{
    auto it = textures_.find(key); //keyをキーとして登録されているデータを検索

    if (it == textures_.end())
    {
        return nullptr;            //ないならnullptrを返す
    }
    return it->second.get();       //あるならそのデータを返す
}

void ResourceManager::Release()
{
    textures_.clear();
}
