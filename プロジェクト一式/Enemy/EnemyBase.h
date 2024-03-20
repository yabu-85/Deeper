#pragma once
#include "../Character/Character.h"
#include "EnemyManager.h"

class EnemyUi;
class StateManager;

class EnemyBase : public Character
{
    void Damage() override;
    void Dead() override;

protected:
    float aimTargetPos_;    //AimTarget‚ÌY²‚Ì‰ÁZ’l
    bool isAimTarget_;      //AimTarget‚É‚µ‚Ä‚¢‚¢‚©‚Ç‚¤‚©
    ENEMY_TYPE type_;

    int actionCoolDown_;    //Move‚Æ‚©‚ÌØ‚è‘Ö‚¦ŠÔ—p‚Ì•Ï”
    int attackDamage_;      //UŒ‚‚Ìƒ_ƒ[ƒW
    int attackCoolDown_;    //UŒ‚‚ÌƒN[ƒ‹ƒ_ƒEƒ“
    float attackDistance_;  //UŒ‚‚ÉˆÚ‚ê‚éTarget‚Æ‚Ì‹——£
    bool isCombatReady_;    //CombatState‚ÉˆÚ‚Á‚Ä‚¢‚¢‚©‚Ç‚¤‚©
    float combatDistance_;  //CombatState‚ÉˆÚ‚Á‚Ä‚¢‚¢‚©‚Ç‚¤‚©‚ÌŒvZ‹——£

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

    //UŒ‚‚Ì€”õ‚ª‚Å‚«‚Ä‚¢‚é‚©
    bool IsAttackReady();

    //AimTarget‚É‚µ‚Ä‚¢‚¢‚©‚Ç‚¤‚©
    bool IsAimTarget() { return isAimTarget_; }

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

