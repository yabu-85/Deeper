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
    
    ENEMY_TYPE type_;       //�G�l�~�[�̃^�C�v
    float aimTargetPos_;    //AimTarget����Y���̉��Z�l
    float combatDistance_;  //CombatState�Ɉڂ��Ă������ǂ����̌v�Z����
    float attackDistance_;  //�U���Ɉڂ��Target�Ƃ̋���
    
    int actionCoolDown_;    //Move�Ƃ��̐؂�ւ����ԗp�̕ϐ�
    int attackCoolDown_;    //�U���̃N�[���_�E��

    bool isAimTarget_;      //AimTarget�ɂ��Ă������ǂ���
    bool isCombatReady_;    //CombatState�Ɉڂ��Ă������ǂ���

public:
    EnemyBase(GameObject* parent, std::string name);
    virtual ~EnemyBase();
    virtual void Initialize() override = 0;
    virtual void Update() override;
    virtual void Draw() override = 0;
    virtual void Release() override;

    virtual void DeadEnter();
    virtual void DeadExit();

    //�U���̏������ł��Ă��邩
    bool IsAttackReady();

    //--------------------�A�N�Z�T-------------------------
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