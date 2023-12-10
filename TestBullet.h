#pragma once
#include "BulletBase.h"

class SphereCollider;
class DamageCtrl;

class TestBullet : public BulletBase
{
    int damage_;
    SphereCollider* collision_;
    DamageCtrl* pDamageCtrl_;

    void KillMe() override;
    void CreatVfx();

public:
    TestBullet(GameObject* parent);
    ~TestBullet() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

