#pragma once
#include "BulletBase.h"

class TestBullet : public BulletBase
{
    int damage_;

public:
    TestBullet(GameObject* parent);
    ~TestBullet() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void OnAttackCollision(GameObject* pTarget);

};

