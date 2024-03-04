#pragma once
#include "../Character.h"
#include "EnemyManager.h"

class EnemyUi;
class StateManager;

enum KNOCK_TYPE {
    SMALL = 0,
    MEDIUM,
    LARGE,
    MAX,
};

class EnemyBase : public Character
{
protected:
    int hp_;                //HP
    int maxHp_;             //HPMAX
    float aimTargetPos_;    //AimTarget����Y���̉��Z�l
    bool isAimTarget_;      //AimTarget�ɂ��Ă������ǂ���
    ENEMY_TYPE type_;

    int actionCoolDown_;    //Move�Ƃ��̐؂�ւ����ԗp�̕ϐ�
    int attackDamage_;      //�U���̃_���[�W
    int attackCoolDown_;    //�U���̃N�[���_�E��
    float attackDistance_;  //�U���Ɉڂ��Target�Ƃ̋���
    bool isCombatReady_;    //CombatState�Ɉڂ��Ă������ǂ���
    float combatDistance_;  //CombatState�Ɉڂ��Ă������ǂ����̌v�Z����

    virtual void SmallKnockBack() {};
    virtual void MediumKnockBack() {};
    virtual void LargetKnockBack() {};

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

    //DeadState�ɓ��������̏���
    void DeadEnter();

    //���S�Ɏ��S
    void Dead();

    //�m�b�N�o�b�N������i����҂��j
    void KnockBack(KNOCK_TYPE type);

    //�U���̏������ł��Ă��邩
    bool IsAttackReady();

    //AimTarget�ɂ��Ă������ǂ���
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

