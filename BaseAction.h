#pragma once

class GameObject;

class BaseAction {
protected:
    GameObject* pGameObject_;

public:
    BaseAction(GameObject* obj);
    virtual ~BaseAction() {};
    virtual void Update() = 0;
    virtual void Initialize() {}
    virtual void Terminate() {}
};