#pragma once
#include "BulletBase.h"

class SphereCollider;

class TestBullet : public BulletBase
{
    int damage_;
    SphereCollider* collision_;

public:
    TestBullet(GameObject* parent);
    ~TestBullet() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

