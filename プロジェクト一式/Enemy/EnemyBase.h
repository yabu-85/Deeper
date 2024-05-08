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
    EnemyUi* pEnemyUi_;
    StateManager* pStateManager_;
    StateManager* pCombatStateManager_;
    
    ENEMY_TYPE type_;       //エネミーのタイプ
    float aimTargetPos_;    //AimTarget時のY軸の加算値
    float combatDistance_;  //CombatStateに移っていいかどうかの計算距離
    float attackDistance_;  //攻撃に移れるTargetとの距離
    
    int actionCoolDown_;    //Moveとかの切り替え時間用の変数
    int attackCoolDown_;    //攻撃のクールダウン

    bool isAimTarget_;      //AimTargetにしていいかどうか
    bool isCombatReady_;    //CombatStateに移っていいかどうか

public:
    EnemyBase(GameObject* parent, std::string name);
    virtual ~EnemyBase();
    virtual void Initialize() override = 0;
    virtual void Update() override;
    virtual void Draw() override = 0;
    virtual void Release() override;

    virtual void DeadEnter();
    virtual void DeadExit();

    //攻撃の準備ができているか
    bool IsAttackReady();

    //--------------------アクセサ-------------------------
    int GetActionCoolDown() { return actionCoolDown_; }
    void SetActionCoolDown(int i) { actionCoolDown_ = i; }
    void SetEnemyType(ENEMY_TYPE type) { type_ = type; }
    ENEMY_TYPE GetEnemyType() { return type_; }
    void SetCombatReady(bool b) { isCombatReady_ = b; }
    bool GetCombatReady() { return isCombatReady_; }

    float GetAimTargetPos() { return aimTargetPos_; }
    void SetAttackCoolDown(int i) { attackCoolDown_ = i; }
    float GetAttackDistance() { return attackDistance_; }
    float GetCombatDistance() { return combatDistance_; }
    bool IsAimTarget() { return isAimTarget_; }

    EnemyUi* GetEnemyUi() { return pEnemyUi_; }
    StateManager* GetStateManager() { return pStateManager_; }
    StateManager* GetCombatStateManager() { return pCombatStateManager_; }
};