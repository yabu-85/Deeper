#pragma once
#include "BulletBase.h"

class PolyLine;

class ThrowBullet : public BulletBase
{
    int time_;                  //�v�Z�p
    float maxHeight_;           //�ő卂�x
    float maxDistance_;         //�ő勗��
    bool isDeath_;              //���S�p�t���O
    PolyLine* pPolyLine_;

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
    void Hit() override;

    void SetThrowData(float maxHeight, float maxDist);
};

