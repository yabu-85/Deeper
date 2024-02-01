#pragma once
#include "Character.h"
#include "EnemyManager.h"

class EnemyUi;
class StateManager;

class EnemyBase : public Character
{
protected:
    int hp_;
    int maxHp_;
    int attackCoolDown_;
    float aimTargetPos_;
    ENEMY_TYPE type_;

    EnemyUi* pEnemyUi_;
    StateManager* pStateManager_;
    StateManager* pCombatStateManager_;

public:
    EnemyBase(GameObject* parent, std::string name);
    virtual ~EnemyBase();
    virtual void Initialize() override = 0;
    virtual void Update() override;
    virtual void Draw() override = 0;
    virtual void Release();

    virtual void ApplyDamage(int da);
    void SetEnemyType(ENEMY_TYPE type) { type_ = type; }
    float GetAimTargetPos() { return aimTargetPos_; }
    
    void SetAttackCoolDown(int i) { attackCoolDown_ = i; }
    bool IsAttackReady();

    EnemyUi* GetEnemyUi() { return pEnemyUi_; }
    StateManager* GetStateManager() { return pStateManager_; }
    StateManager* GetCombatStateManager() { return pCombatStateManager_; }
};

