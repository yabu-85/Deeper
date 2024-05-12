#include "EnemyAttackSelect.h"
#include <mutex>
#include <cassert>
#include <algorithm>

void SelectoAttack::Selector(EnemyBase* enemy)
{
    //�g�p�ȋZ������
    std::vector<int> availables;
    for (int i = 0; i < attacks_.size(); i++) {
        //�g�p�\�̏ꍇ���X�g�ɒǉ�
        if (attacks_.at(i)->CanUseAttack(enemy)) availables.push_back(i);
    }

    //�g�p�\�ȋZ���Ȃ�����I���
    if (availables.empty()) return;

    //priority�ŏ����\�[�g
    std::sort(availables.begin(), availables.end(), [&](int a, int b) {
        return attacks_[a]->GetPriority() < attacks_[b]->GetPriority();
        });

    //��ԗD��x�����U���������邩�v�Z
    int selectSize = 0;
    int selectPrio = attacks_[availables.front()]->GetPriority();
    for (int i : availables) {
        if (attacks_[availables.at(i)]->GetPriority() == selectPrio) selectSize++;
        else break;
    }

    //�����_���őI��
    int randSelect = rand() % selectSize;
    selectAttack_ = availables[randSelect];
}

void SelectoAttack::AddSelectAttack(SelectAttackInfo* info)
{
    attacks_.push_back(info);
}