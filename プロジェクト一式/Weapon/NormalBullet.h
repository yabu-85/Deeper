#pragma once
#include "BulletBase.h"

/// <summary>
/// 直線移動する弾丸
/// </summary>
class NormalBullet : public BulletBase
{
public:
    NormalBullet(GameObject* parent);
    ~NormalBullet() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void OnAttackCollision(GameObject* pTarget);
    void Hit() override;

};

