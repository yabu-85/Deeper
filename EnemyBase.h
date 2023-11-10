#pragma once
#include "Engine/GameObject.h"
#include "EnemySpawnCtrl.h"

class EnemyUi;
class DropTable;
class StateManager;

class EnemyBase : public GameObject
{

protected:
    int hp_;
    int maxHp_;
    XMFLOAT3 targetPos_;
    ENEMY_TYPE type_;
    EnemyUi* pEnemyUi_;

public:
    EnemyBase(GameObject* parent);
    virtual ~EnemyBase() = 0 {};
    virtual void Initialize() override = 0 {};
    virtual void Update() override = 0 {};
    virtual void Draw() override = 0 {};
    virtual void Release() override = 0 {};

    virtual void ApplyDamage(int da);
    void SetEnemyType(ENEMY_TYPE type) { type_ = type; }
    XMFLOAT3 GetTargetPos() { return targetPos_; }
};

