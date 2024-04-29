#pragma once
#include "../Character/Character.h"
#include "EnemyManager.h"

class EnemyUi;
class StateManager;

class EnemyBase : public Character
{
    virtual void Damage() override;
    virtual void Dead() override;

protected:
    bool isAimTarget_;      //AimTargetにしていいかどうか
    ENEMY_TYPE type_;       //
    float aimTargetPos_;    //AimTarget時のY軸の加算値

    int actionCoolDown_;    //Moveとかの切り替え時間用の変数
    int attackCoolDown_;    //攻撃のクールダウン
    float attackDistance_;  //攻撃に移れるTargetとの距離
    bool isCombatReady_;    //CombatStateに移っていいかどうか
    float combatDistance_;  //CombatStateに移っていいかどうかの計算距離

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

    virtual void DeadEnter();
    virtual void DeadExit();

    //攻撃の準備ができているか
    bool IsAttackReady();

    //AimTargetにしていいかどうか
    bool IsAimTarget() { return isAimTarget_; }

    int GetActionCoolDown() { return actionCoolDown_; }
    void SetActionCoolDown(int i) { actionCoolDown_ = i; }
    float GetAimTargetPos() { return aimTargetPos_; }
    void SetAttackCoolDown(int i) { attackCoolDown_ = i; }
    float GetAttackDistance() { return attackDistance_; }
    void SetEnemyType(ENEMY_TYPE type) { type_ = type; }
    ENEMY_TYPE GetEnemyType() { return type_; }

    void SetCombatReady(bool b) { isCombatReady_ = b; }
    bool GetCombatReady() { return isCombatReady_; }
    float GetCombatDistance() { return combatDistance_; }

    EnemyUi* GetEnemyUi() { return pEnemyUi_; }
    StateManager* GetStateManager() { return pStateManager_; }
    StateManager* GetCombatStateManager() { return pCombatStateManager_; }
};