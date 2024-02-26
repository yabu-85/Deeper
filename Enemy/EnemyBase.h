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
    int attackDamage_;      //�U���̃_���[�W
    int attackCoolDown_;    //�U���̃N�[���_�E��
    float aimTargetPos_;    //AimTarget����Y���̉��Z�l
    ENEMY_TYPE type_;

    bool isCombatReady_;    //CombatState�Ɉڂ��Ă������ǂ���
    float combatDistance_;  //CombatState�Ɉڂ��Ă������ǂ����̌v�Z����

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

    float GetAimTargetPos() { return aimTargetPos_; }
    void SetAttackDamage(int i) { attackDamage_ = i; }
    int GetAttackDamage() { return attackDamage_; }
    void SetAttackCoolDown(int i) { attackCoolDown_ = i; }
    bool IsAttackReady();

    void SetEnemyType(ENEMY_TYPE type) { type_ = type; }
    ENEMY_TYPE GetEnemyType() { return type_; }

    void SetCombatReady(bool b) { isCombatReady_ = b; }
    bool GetCombatReady() { return isCombatReady_; }
    float GetCombatDistance() { return combatDistance_; }

    EnemyUi* GetEnemyUi() { return pEnemyUi_; }
    StateManager* GetStateManager() { return pStateManager_; }
    StateManager* GetCombatStateManager() { return pCombatStateManager_; }
};

