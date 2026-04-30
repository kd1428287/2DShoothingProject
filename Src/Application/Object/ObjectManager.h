#pragma once

class BaseObject;

enum class ObjectType
{
    Player,
    Enemy,
    Prop
};

class ObjectManager
{
public:
    static ObjectManager& Instance()
    {
        static ObjectManager instance;
        return instance;
    }

    void Add(std::unique_ptr<BaseObject> obj);

    void Add(ObjectType type);
    void Add(ObjectType type, Math::Vector2 pos);

    void PreUpdate(float dt);
    void Update(float dt);
    void DrawRequest();

private:
    ObjectManager();
    ~ObjectManager();

    std::vector<std::unique_ptr<BaseObject>> objects_;

    std::vector<std::unique_ptr<BaseObject>> pendingObjects_;
};