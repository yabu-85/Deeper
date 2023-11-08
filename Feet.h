#pragma once
#include "EnemyBase.h"
#include "StateBase.h"

class NavigationAI;

//Ç†ÇµÅFNavigationÇ∆Ç©AIäJî≠ópÇÃìGÇ‡Ç‘
class Feet : public EnemyBase
{
    int hModel_;
    NavigationAI* pNavigationAI_;

    XMFLOAT3 targetPos_;
    int stopTime_;
    int currtime_;
    bool flag_;

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
public:
    FeetWait(StateManager* owner);
    const char* GetName() const override { return "Wait"; }
    void Update() override;
};