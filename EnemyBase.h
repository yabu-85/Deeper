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
    EnemyUi* pEnemyUi_;
    ENEMY_TYPE type_;
    StateManager* pStateManager_;

public:
    EnemyBase(GameObject* parent) : GameObject(parent), pEnemyUi_(nullptr), hp_(0), maxHp_(0), type_(ENEMY_MAX), pStateManager_(nullptr) {};
    virtual ~EnemyBase() = 0 {};
    virtual void Initialize() override = 0 {};
    virtual void Update() override = 0 {};
    virtual void Draw() override = 0 {};
    virtual void Release() override = 0 {};

    virtual void ApplyDamage(int da) = 0;
    void SetEnemyType(ENEMY_TYPE type) { type_ = type; }
};

