#pragma once

class ResourceManager
{
public:
    static ResourceManager& Instance();

    // ===== Texture =====
    void LoadTexture(const std::string& key,
        const std::string& filePath);
    KdTexture* GetTexture(const std::string& key) const;

    void Release();


private:
    ResourceManager() {};

    std::unordered_map<std::string, std::unique_ptr<KdTexture>> textures_;
};

#define RESOURCE ResourceManager::Instance()