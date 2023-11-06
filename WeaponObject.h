#pragma once
#include "Engine/GameObject.h"

class WeaponObject : public GameObject
{
    int hModel_;

public:
    WeaponObject(GameObject* parent);
    ~WeaponObject() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

