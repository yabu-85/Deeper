#pragma once
#include "WeaponBase.h"
#include "StateBase.h"
class DamageCtrl;
class Player;

class TestWeaponMain : public WeaponBase
{
public:
    TestWeaponMain(GameObject* parent);
    ~TestWeaponMain() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void ResetState() override;
};

//-------------------State----------------------

class TestWeaponWait : public StateBase
{
    WeaponBase* pWeaponBase_;
public:
    TestWeaponWait(StateManager* owner);
    const char* GetName() const override { return "Wait"; }
    void Update() override;
};

class TestWeaponCombo1 : public StateBase
{
    bool next_;
    int time_;
    WeaponBase* pWeaponBase_;
    DamageCtrl* pDamageCtrl_;
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
    WeaponBase* pWeaponBase_;
    DamageCtrl* pDamageCtrl_;
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
    WeaponBase* pWeaponBase_;
    DamageCtrl* pDamageCtrl_;
    Player* pPlayer_;
public:
    TestWeaponCombo3(StateManager* owner);
    const char* GetName() const override { return "Combo3"; }
    void Update() override;
    void OnEnter() override;
    void OnExit() override;
};