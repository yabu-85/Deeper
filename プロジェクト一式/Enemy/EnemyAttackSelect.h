#pragma once
#include <vector>
#include "../Engine/Singleton.h"
class EnemyBase;

class SelectAttackInfo
{
    int priority_;   //�����U���\���������̗D��x
public:
    SelectAttackInfo() : priority_(0) {}
    virtual bool CanUseAttack(EnemyBase* enemy) { return false; }
    int GetPriority() { return priority_; }
    void SetPriority(int p) { priority_ = p; }
};

#include <map>

class SelectAttack {
    int selectAttack_;
    std::vector<int> comboHistory_;
    std::vector<SelectAttackInfo*> attacks_;

    void SelectNoCombo(EnemyBase* e, std::vector<int>& list);
    void SelectCombo(EnemyBase* e, std::vector<int>& list);


    int currentAttackType_; // ���݂̍U���^�C�v
    std::map<int, std::vector<SelectAttackInfo*>> attackMap_; // �U���^�C�v�ɑ΂���h���\�ȍU���̃}�b�v
    void Test() {
        // ���݂̍U���^�C�v�ɑ΂���h���\�ȍU�����擾
        const auto& availableAttacks = attackMap_[currentAttackType_];


    }
    void AddSelectAttack(int attackType, SelectAttackInfo* info) {
        attackMap_[attackType].push_back(info);
    }


public:
    SelectAttack();

    //�U�����̒ǉ�
    void AddSelectAttack(SelectAttackInfo* info);
    
    //�I�ׂ邩���v�Z���邾���i�擾��GetSelectAttack�j
    bool Selector(EnemyBase* enemy);

    //�U���̔ԍ��擾
    int GetSelectAttack() { return selectAttack_; }

    //�R���{��������
    void AddToComboHistory(int attackType) { comboHistory_.push_back(attackType); }
    
    //�R���{���N���A
    void ClearComboHistory() { comboHistory_.clear(); }
};
