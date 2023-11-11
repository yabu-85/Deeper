#pragma once
#include "Engine/GameObject.h"
#include "EnemySpawnCtrl.h"

class EnemyUi;
class DropTable;
class StateManager;

class EnemyBase : public GameObject
{
public:
    enum class State {
        IDLE = 0,
        TARGET,
        DEAD,
    };

protected:
    int hp_;
    int maxHp_;
    State state_;
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
    
    State IsState() { return state_; }
    void SetState(State state) { state_ = state; }
};

