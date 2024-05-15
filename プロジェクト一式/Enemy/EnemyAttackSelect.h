#pragma once
#include <vector>
#include "../Engine/Singleton.h"
class EnemyBase;

class SelectAttackInfo
{
    int priority_;                  //�����U���\���������̗D��x
    std::vector<int> comboList_;    //�h���\�U���ԍ�
public:
    SelectAttackInfo() : priority_(0) {}
    virtual bool CanUseAttack(EnemyBase* enemy) { return false; }
    int GetPriority() { return priority_; }
    void SetPriority(int p) { priority_ = p; }

    std::vector<int>& GetComboList() { return comboList_; }
    void AddComboList(int i) { comboList_.push_back(i); }
    void SetComboList(std::vector<int> list) { comboList_ = list; }
};

class SelectAttack {
    int currentAttack_;                 //���ݑI��ł�U��
    std::vector<int> comboHistory_;     //���܂ł̃R���{����
    void SelectNoCombo(EnemyBase* e, std::vector<int>& list);
    void SelectCombo(EnemyBase* e, std::vector<int>& list);
    
    //�U����񃊃X�g
    std::vector<SelectAttackInfo*> attacks_;

public:
    SelectAttack();

    //�U�����̒ǉ�
    void AddSelectAttack(SelectAttackInfo* info);
    
    //�I�ׂ邩���v�Z���邾���i�擾��GetSelectAttack�j
    bool Selector(EnemyBase* enemy);

    //�U���̔ԍ��擾
    int GetSelectAttack() { return currentAttack_; }

    //�R���{�����ǂ���
    bool IsCombo() { return !comboHistory_.empty(); }

    //�R���{��������
    void AddComboHistory(int attackType) { comboHistory_.push_back(attackType); }
    
    //�R���{���N���A
    void ClearComboHistory() { comboHistory_.clear(); }
};
