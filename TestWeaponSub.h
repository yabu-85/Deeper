#pragma once
#include "WeaponBase.h"
#include "StateBase.h"

class Player;

class TestWeaponSub : public WeaponBase
{
    Player* pPlayer_;
public:
    TestWeaponSub(GameObject* parent);
    ~TestWeaponSub() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void ResetState() override;
};

//-------------------State----------------------

class TestWeaponSubWait : public StateBase
{
    WeaponBase* pWeaponBase_;
public:
    TestWeaponSubWait(StateManager* owner);
    const char* GetName() const override { return "Wait"; }
    void Update() override;
};

class TestWeaponSubCombo1 : public StateBase
{
    bool next_;
    int time_;
    WeaponBase* pWeaponBase_;
    Player* pPlayer_;
public:
    TestWeaponSubCombo1(StateManager* owner);
    const char* GetName() const override { return "Combo1"; }
    void Update() override;
    void OnEnter() override;
    void OnExit() override;
};

class TestWeaponSubCombo2 : public StateBase
{
    bool next_;
    int time_;
    WeaponBase* pWeaponBase_;
    Player* pPlayer_;
public:
    TestWeaponSubCombo2(StateManager* owner);
    const char* GetName() const override { return "Combo2"; }
    void Update() override;
    void OnEnter() override;
    void OnExit() override;
};
