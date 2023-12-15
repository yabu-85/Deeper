#pragma once
#include "BulletBase.h"

class SphereCollider;
class DamageManager;

class TestBullet : public BulletBase
{
    int damage_;
    SphereCollider* collision_;
    DamageManager* pDamageManager_;

    void CreatVfx();

public:
    TestBullet(GameObject* parent);
    ~TestBullet() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

