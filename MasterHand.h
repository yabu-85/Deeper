#pragma once
#include "EnemyBase.h"

//�܂��e�X�g�p����
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
    void ApplyDamage(int da) override;

};

