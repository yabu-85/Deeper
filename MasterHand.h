#pragma once
#include "EnemyBase.h"

class NavigationAI;

class MasterHand : public EnemyBase
{
    int hModel_;
    int lifeTime_;

    NavigationAI* pNavigationAI_;

public:
    MasterHand(GameObject* parent);
    ~MasterHand() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

