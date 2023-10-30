#pragma once
#include "BulletBase.h"

class TestBullet : public BulletBase
{
public:
    TestBullet(GameObject* parent);
    ~TestBullet() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

