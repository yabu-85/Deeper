#pragma once
#include "WeaponBase.h"
#include "../State/StateBase.h"

class Player;
class SphereCollider;
class DoublePolyLine;
class DamageController;

class MainSwordWeapon : public WeaponBase
{
    XMFLOAT3 wandPos_;      //プレイヤーの手のポジション
    XMFLOAT3 direction_;    //剣先の進行方向

    Player* pPlayer_;
    SphereCollider* sphere_[2];
    DoublePolyLine* pDoublePolyLine_;
    DamageController* pDamageController_;

public:
    MainSwordWeapon(GameObject* parent);
    ~MainSwordWeapon() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void OnAttackCollision(GameObject* pTarget) override;
    void ChangeAttackState() override;
    void ResetState() override;

    //攻撃判定コライダーの計算
    void CalcSwordTrans();

    void DamageInfoReset();
    void SetDamageInfoCombo1();
    void SetDamageInfoCombo2();
    void SetDamageInfoCombo3();

    DoublePolyLine* GetPolyLine() { return pDoublePolyLine_; }

};

//-------------------State----------------------

class MainSwordWeaponCombo1 : public StateBase
{
    bool next_;
    int time_;
public:
    MainSwordWeaponCombo1(StateManager* owner);
    const char* GetName() const override { return "Combo1"; }
    void Update() override;
    void OnEnter() override;
    void OnExit() override;
};

class MainSwordWeaponCombo2 : public StateBase
{
    bool next_;
    int time_;
public:
    MainSwordWeaponCombo2(StateManager* owner);
    const char* GetName() const override { return "Combo2"; }
    void Update() override;
    void OnEnter() override;
    void OnExit() override;
};

class MainSwordWeaponCombo3 : public StateBase
{
    bool next_;
    int time_;
public:
    MainSwordWeaponCombo3(StateManager* owner);
    const char* GetName() const override { return "Combo3"; }
    void Update() override;
    void OnEnter() override;
    void OnExit() override;
};