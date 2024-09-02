#pragma once
#include "../Engine/GameObject.h"

/// <summary>
/// 回復するアイテム
/// 敵を倒すとドロップして、拾うと回復する
/// </summary>
class RecoveryItem : public GameObject
{
    int hModel_;            //モデル
    int recoveryAmount_;    //回復量
    float gravity_;         //重力
public:
    RecoveryItem(GameObject* parent);
    ~RecoveryItem();
    void Initialize();
    void Update();
    void Draw();
    void Release();
    void OnCollision(GameObject* pTarget) override;
};

