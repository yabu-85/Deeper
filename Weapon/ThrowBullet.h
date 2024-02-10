#pragma once
#include "BulletBase.h"

class ThrowBullet : public BulletBase
{
    int damage_;
    int time_;          //�v�Z�p
    float maxHeight_;   //�ő卂�x
    float maxDistance_; //�ő勗��

public:
    ThrowBullet(GameObject* parent);
    ~ThrowBullet() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void OnAttackCollision(GameObject* pTarget);
    
    void SetThrowData(float maxHeight, float maxDist);
    void Shot(XMFLOAT3 pos, XMFLOAT3 target) override;
    void Move() override;

};

