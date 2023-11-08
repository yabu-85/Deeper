#pragma once
#include "EnemyBase.h"
#include "StateBase.h"

//Ç†ÇµÅFNavigationÇ∆Ç©AIäJî≠ópÇÃìGÇ‡Ç‘
class Feet : public EnemyBase
{
    int hModel_;

public:
    Feet(GameObject* parent);
    ~Feet() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void ApplyDamage(int da) override;

};

//-------------------State----------------------

class FeetWait : public StateBase
{
    int time_;
    Feet* pFeet_;
public:
    FeetWait(StateManager* owner);
    const char* GetName() const override { return "Wait"; }
    void Update() override;
    void OnEnter() override;
};

class FeetWalk : public StateBase
{
    Feet* pFeet_;
    XMFLOAT3 targetPos_;
public:
    FeetWalk(StateManager* owner);
    const char* GetName() const override { return "Wait"; }
    void Update() override;
    void OnEnter() override;
};

class FeetDead : public StateBase
{
    Feet* pFeet_;
public:
    FeetDead(StateManager* owner);
    const char* GetName() const override { return "Wait"; }
    void Update() override;
    void OnEnter() override;
};