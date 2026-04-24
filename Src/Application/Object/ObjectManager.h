#pragma once

class BaseObject;

class ObjectManager
{
public:

    ObjectManager();
    ~ObjectManager();

    void Add(std::unique_ptr<BaseObject> obj);

    void PreUpdate(float dt);
    void Update(float dt);
    void DrawRequest();

private:
    std::vector<std::unique_ptr<BaseObject>> objects_;

    std::vector<std::unique_ptr<BaseObject>> pendingObjects_;
};