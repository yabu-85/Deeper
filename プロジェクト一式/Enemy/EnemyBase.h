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
    float aimTargetPos_;    //AimTarget����Y���̉��Z�l
    bool isAimTarget_;      //AimTarget�ɂ��Ă������ǂ���
    ENEMY_TYPE type_;

    int actionCoolDown_;    //Move�Ƃ��̐؂�ւ����ԗp�̕ϐ�
    int attackCoolDown_;    //�U���̃N�[���_�E��
    float attackDistance_;  //�U���Ɉڂ��Target�Ƃ̋���
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

    virtual void DeadEnter();
    virtual void DeadExit();

    //�U���̏������ł��Ă��邩
    bool IsAttackReady();

    //AimTarget�ɂ��Ă������ǂ���
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

#include <vector>
#include <array>
using std::vector;
using std::pair;
class EnemyBase;

class EnemyFrame {
    pair<int, int> frame;
    bool inFrame_;
    bool isValid_;

protected:
    EnemyBase* pParent_;

public:
    EnemyFrame(int start, int end);
    void SetParent(EnemyBase* enemy) { pParent_ = enemy; }
    bool IsTargetFrame(int time) { return frame.first >= time && frame.second <= time; }
    bool IsEndFrame(int time) { return frame.second == time; }
    
    bool GetInFrame() { return inFrame_; }
    void SetInFrame(bool b) { inFrame_ = b; }
    bool GetValid() { return isValid_; }
    void SetValid(bool b) { isValid_ = b; }

    virtual void Update() {};
    virtual void OnEnter() {};
    virtual void OnExit() {};
};

class EnemyAttackController {
    std::vector<EnemyFrame*> actions;

public:
    ~EnemyAttackController();
    void AddAction(EnemyFrame* action) { actions.push_back(action); }

    //�o�^���ꂽ�A�N�V����������t���[����������Action�Ă�
    void Update(int currentTime);
};