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
    int attackDamage_;      //攻撃のダメージ
    int attackCoolDown_;    //攻撃のクールダウン
    float aimTargetPos_;    //AimTarget時のY軸の加算値
    float combatDistance_;  //CombatStateに移っていいかどうかの距離
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
    
    float GetAimTargetPos() { return aimTargetPos_; }
    float GetCombatDistance() { return combatDistance_; }
    void SetEnemyType(ENEMY_TYPE type) { type_ = type; }
    ENEMY_TYPE GetEnemyType() { return type_; }
    void SetAttackDamage(int i) { attackDamage_ = i; }
    int GetAttackDamage() { return attackDamage_; }
    void SetAttackCoolDown(int i) { attackCoolDown_ = i; }
    bool IsAttackReady();

    EnemyUi* GetEnemyUi() { return pEnemyUi_; }
    StateManager* GetStateManager() { return pStateManager_; }
    StateManager* GetCombatStateManager() { return pCombatStateManager_; }
};

