#pragma once
#include "WeaponBase.h"
#include "../State/StateBase.h"

class Player;
class SegmentCollider;
class PolyLine;

class TestWeaponMain : public WeaponBase
{
    int damage_;
    XMFLOAT3 wandPos_;
    Player* pPlayer_;
    SegmentCollider* seg_;
    PolyLine* pPolyLine_;

public:
    TestWeaponMain(GameObject* parent);
    ~TestWeaponMain() override;
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

class TestWeaponWait : public StateBase
{
public:
    TestWeaponWait(StateManager* owner);
    const char* GetName() const override { return "Wait"; }
    void Update() override;
};

class TestWeaponCombo1 : public StateBase
{
    bool next_;
    int time_;
    int comboTime_;
    Player* pPlayer_;
public:
    TestWeaponCombo1(StateManager* owner);
    const char* GetName() const override { return "Combo1"; }
    void Update() override;
    void OnEnter() override;
    void OnExit() override;
};

class TestWeaponCombo2 : public StateBase
{
    bool next_;
    int time_;
    int comboTime_;
    Player* pPlayer_;
public:
    TestWeaponCombo2(StateManager* owner);
    const char* GetName() const override { return "Combo2"; }
    void Update() override;
    void OnEnter() override;
    void OnExit() override;
};

class TestWeaponCombo3 : public StateBase
{
    bool next_;
    int time_;
    int comboTime_;
    Player* pPlayer_;
public:
    TestWeaponCombo3(StateManager* owner);
    const char* GetName() const override { return "Combo3"; }
    void Update() override;
    void OnEnter() override;
    void OnExit() override;
};