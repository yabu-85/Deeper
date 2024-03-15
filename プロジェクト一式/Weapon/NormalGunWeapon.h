#pragma once
#include "WeaponBase.h"
#include "../State/StateBase.h"

class Player;

class NormalGunWeapon : public WeaponBase
{
    Player* pPlayer_;
public:
    NormalGunWeapon(GameObject* parent);
    ~NormalGunWeapon() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void ResetState() override;
    void ChangeAttackState() override;

    void ShotBullet();
};

//-------------------State----------------------

class NormalGunWeaponCombo1 : public StateBase
{
    int time_;
public:
    NormalGunWeaponCombo1(StateManager* owner);
    const char* GetName() const override { return "Combo1"; }
    void Update() override;
    void OnEnter() override;
    void OnExit() override;
};