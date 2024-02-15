#pragma once
#include "../Engine/GameObject.h"

class PlayerOrnament;

class WeaponMainOrnament : public GameObject
{
    int hModel_;
    int boneIndex_;
    int partIndex_;
    XMFLOAT3 wandPos_;
    PlayerOrnament* pPlayerOrnament_;

public:
    WeaponMainOrnament(GameObject* parent);
    ~WeaponMainOrnament() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};