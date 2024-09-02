#pragma once
#include "BulletBase.h"

/// <summary>
/// ’¼üˆÚ“®‚·‚é’eŠÛ
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

