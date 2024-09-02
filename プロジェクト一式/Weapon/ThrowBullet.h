#pragma once
#include "BulletBase.h"

class PolyLine;

/// <summary>
/// 弧を描いて飛ぶ弾丸
/// 最大距離と高さを設定できる
/// </summary>
class ThrowBullet : public BulletBase
{
    int time_;                  //計算用
    float maxHeight_;           //最大高度
    float maxDistance_;         //最大距離
    bool isDeath_;              //死亡用フラグ
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

