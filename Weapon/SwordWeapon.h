#pragma once
#include "WeaponBase.h"
#include "../State/StateBase.h"

class Player;
class SegmentCollider;
class PolyLine;

class SwordWeapon : public WeaponBase
{
    int damage_;
    XMFLOAT3 wandPos_;
    Player* pPlayer_;
    SegmentCollider* seg_;
    PolyLine* pPolyLine_;

public:
    SwordWeapon(GameObject* parent);
    ~SwordWeapon() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void OnAttackCollision(GameObject* pTarget) override;

    void ResetState() override;
    void CalcDamage();

    PolyLine* GetPolyLine() { return pPolyLine_; }
    SegmentCollider* GetSegmentCollider() { return seg_; }
};

//-------------------State----------------------


class SwordWeaponWait : public StateBase
{
public:
    SwordWeaponWait(StateManager* owner);
    const char* GetName() const override { return "Wait"; }
    void Update() override;
};

class SwordWeaponCombo1 : public StateBase
{
    bool next_;
    int time_;
    int comboTime_;
    Player* pPlayer_;
public:
    SwordWeaponCombo1(StateManager* owner);
    const char* GetName() const override { return "Combo1"; }
    void Update() override;
    void OnEnter() override;
    void OnExit() override;
};

class SwordWeaponCombo2 : public StateBase
{
    bool next_;
    int time_;
    int comboTime_;
    Player* pPlayer_;
public:
    SwordWeaponCombo2(StateManager* owner);
    const char* GetName() const override { return "Combo2"; }
    void Update() override;
    void OnEnter() override;
    void OnExit() override;
};