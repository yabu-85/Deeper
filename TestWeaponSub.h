#pragma once
#include "WeaponBase.h"
#include "StateBase.h"

class TestWeaponSub : public WeaponBase
{
public:
    TestWeaponSub(GameObject* parent);
    ~TestWeaponSub() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void ResetState() override;

};