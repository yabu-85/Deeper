#pragma once
#include "WeaponBase.h"
#include "../State/StateBase.h"

class Player;
class SphereCollider;

class StoneArmWeapon : public WeaponBase
{
    int atkBoneIndex_;
    int atkPartIndex_;
    XMFLOAT3 atkPosition_;
    Player* pPlayer_;
    SphereCollider* pHandCollider_;

public:
    StoneArmWeapon(GameObject* parent);
    ~StoneArmWeapon() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void ResetState() override;
    void ChangeAttackState() override;
    void OnAttackCollision(GameObject* pTarget) override;

};

//-------------------State----------------------

class StoneArmWeaponCombo1 : public StateBase
{
    bool next_;
    int time_;
public:
    StoneArmWeaponCombo1(StateManager* owner);
    const char* GetName() const override { return "Combo1"; }
    void Update() override;
    void OnEnter() override;
    void OnExit() override;
};