#pragma once
#include "../Engine/GameObject.h"

class PlayerOrnament : public GameObject
{
    int hModel_;

public:
    PlayerOrnament(GameObject* parent);
    ~PlayerOrnament();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    int GetModelHandle() { return hModel_; }

};

