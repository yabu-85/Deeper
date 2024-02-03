#pragma once
#include "../Engine/GameObject.h"
#include <vector>

class UIManager;

class SceneBase : public GameObject
{

protected:
	std::vector<UIManager*> pUIManagerList_;

public:
	SceneBase(GameObject* parent, std::string name);
    virtual void Initialize() override = 0 {};
    virtual void Update() override = 0 {};
    virtual void Draw() override = 0 {};
    virtual void Release() override = 0 {};
    void AddUIManager(UIManager* ma);

};

