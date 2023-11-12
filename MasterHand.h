#pragma once
#include "EnemyBase.h"

//まじテスト用もぶ
class MasterHand : public EnemyBase
{
    int hModel_;
    int lifeTime_;

public:
    MasterHand(GameObject* parent);
    ~MasterHand() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

