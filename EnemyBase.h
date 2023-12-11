#pragma once
#include "Character.h"
#include "EnemySpawnCtrl.h"

class EnemyUi;
class DropTable;
class StateManager;

class EnemyBase : public Character
{
protected:
    float aimTargetPos_;        //Aim‚ÌTarget‚·‚é‚ÌêŠ
    ENEMY_TYPE type_;

    EnemyUi* pEnemyUi_;
    StateManager* pStateManager_;
    StateManager* pCombatStateManager_;

public:
    EnemyBase(GameObject* parent);
    virtual ~EnemyBase();
    virtual void Initialize() override = 0 {};
    virtual void Update() override = 0 {};
    virtual void Draw() override = 0 {};
    virtual void Release() override = 0 {};

    virtual void ApplyDamage(int da) override;
    void SetEnemyType(ENEMY_TYPE type) { type_ = type; }
    float GetAimTargetPos() { return aimTargetPos_; }

    EnemyUi* GetEnemyUi() { return pEnemyUi_; }
    StateManager* GetStateManager() { return pStateManager_; }
    StateManager* GetCombatStateManager() { return pCombatStateManager_; }
};

