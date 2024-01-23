#pragma once
#include "Engine/GameObject.h"
#include "PlayerCommand.h"

class ActionImage : public GameObject
{
public:

private:
    std::string keyName_[10];

public:
    ActionImage(GameObject* parent);
    ~ActionImage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void SetKeyName();
    void DrawActionUI();
    void SetDrawActionUI();

};

