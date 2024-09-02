#pragma once
#include "BaseAction.h"
#include <DirectXMath.h>

using namespace DirectX;

/// <summary>
/// コリジョンマップを使って計算して、視点での探索をする
/// </summary>
class VisionSearchAction : public BaseAction
{
    bool isFoundTarget_;    //ターゲットを見つけたか
    Character* pTarget_;    //ターゲットのポインタ
    float range_;           //視野の範囲
    float fovRadian_;       //視野の半径

public:
    VisionSearchAction(Character* obj, float range, float fov);
    virtual ~VisionSearchAction() override {};
    virtual void Update() override;
    
    void SetTarget(Character* tar) { pTarget_ = tar; }
    bool IsFoundTarget() { return isFoundTarget_; }

};