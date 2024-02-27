#pragma once
#include "../Character.h"
#include "EnemyManager.h"

class EnemyUi;
class StateManager;

class EnemyBase : public Character
{
protected:
    int hp_;                //HP
    int maxHp_;             //HPMAX
    float aimTargetPos_;    //AimTarget時のY軸の加算値
    ENEMY_TYPE type_;

    int actionCoolDown_;    //Moveとかの切り替え時間用の変数
    int attackDamage_;      //攻撃のダメージ
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
    virtual void ApplyDamage(int da);
    void Dead();
    bool IsAttackReady();

    int GetActionCoolDown() { return actionCoolDown_; }
    void SetActionCoolDown(int i) { actionCoolDown_ = i; }
    float GetAimTargetPos() { return aimTargetPos_; }
    void SetAttackDamage(int i) { attackDamage_ = i; }
    int GetAttackDamage() { return attackDamage_; }
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

