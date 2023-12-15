#pragma once
#include "WeaponBase.h"
#include "StateBase.h"

class DamageManager;
class Player;
class LineCollider;

class TestWeaponMain : public WeaponBase
{
    int damage_;
    XMFLOAT3 wandPos_;
    Player* pPlayer_;
    DamageManager* pDamageManager_;
    LineCollider* line_;

public:
    TestWeaponMain(GameObject* parent);
    ~TestWeaponMain() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void ResetState() override;
    void CalcDamage(float range = 10.0);
};

//-------------------State----------------------

class TestWeaponWait : public StateBase
{
    TestWeaponMain* pTestWeaponMain_;
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
    TestWeaponMain* pTestWeaponMain_;
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
    TestWeaponMain* pTestWeaponMain_;
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
    TestWeaponMain* pTestWeaponMain_;
    Player* pPlayer_;
public:
    TestWeaponCombo3(StateManager* owner);
    const char* GetName() const override { return "Combo3"; }
    void Update() override;
    void OnEnter() override;
    void OnExit() override;
};