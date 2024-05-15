#include "EnemyAttackSelect.h"
#include <algorithm>

SelectAttack::SelectAttack() : currentAttack_(0)
{
}

bool SelectAttack::Selector(EnemyBase* enemy)
{
    //�g�p�\�ȋZ�̌v�Z
    std::vector<int> availables;
    if (comboHistory_.empty()) SelectNoCombo(enemy, availables);
    else SelectCombo(enemy, availables);



    //�g�p�\�ȋZ���Ȃ�����I���
    if (availables.empty()) return false;

    //priority�ŏ����\�[�g
    std::sort(availables.begin(), availables.end(), [&](int a, int b) {
        return attacks_[a]->GetPriority() < attacks_[b]->GetPriority();
    });

    //��ԗD��x�����U���������邩�v�Z
    int selectSize = 0;
    int selectPrio = attacks_[availables.front()]->GetPriority();
    for (int i : availables) {
        if (attacks_[i]->GetPriority() == selectPrio) selectSize++;
        else break;
    }

    //�����_���őI��
    int randSelect = rand() % selectSize;
    currentAttack_ = availables[randSelect];
    return true;
}

void SelectAttack::AddSelectAttack(SelectAttackInfo* info)
{
    attacks_.push_back(info);
}

void SelectAttack::SelectNoCombo(EnemyBase* e, std::vector<int>& list)
{
    for (int i = 0; i < attacks_.size(); i++) {
        //�g�p�\�̏ꍇ���X�g�ɒǉ�
        if (attacks_.at(i)->CanUseAttack(e)) list.push_back(i);
    }
}

void SelectAttack::SelectCombo(EnemyBase* e, std::vector<int>& list)
{
    std::vector<int> combo = attacks_.at(currentAttack_)->GetComboList();
    for (int i = 0; i < attacks_.size(); i++) {
        //�R���{�h���Ώۉ��ǂ���
        if (std::find(combo.begin(), combo.end(), i) == combo.end()) continue;

        //���łɃR���{�U�����Ă��邩�ǂ���
        if (std::find(comboHistory_.begin(), comboHistory_.end(), i) != comboHistory_.end()) continue;
       
        //�U���ł���Ȃ�ǉ�
        if (attacks_.at(i)->CanUseAttack(e)) list.push_back(i);
    }

    //�����I�ׂȂ�������R���{���Z�b�g��
    if (list.empty()) ClearComboHistory();
}