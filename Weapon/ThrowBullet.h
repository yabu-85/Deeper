#pragma once
#include "BulletBase.h"

class ThrowBullet : public BulletBase
{
    int damage_;
    XMVECTOR vVelocity_;     //‚È‚ñ‚©“Š‚°‚é‚â‚Â‚ÌŒvŽZ—p‚Ý‚½‚¢

public:
    ThrowBullet(GameObject* parent);
    ~ThrowBullet() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void OnAttackCollision(GameObject* pTarget);

    void Shot(XMFLOAT3 pos, XMFLOAT3 target) override;
    void Move() override;

};

