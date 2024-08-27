#include "EnemyAttackSelect.h"
#include <algorithm>

SelectAttack::SelectAttack() : currentAttack_(0)
{
}

bool SelectAttack::Selector(EnemyBase* enemy)
{
    //使用可能な技の計算
    std::vector<int> availables;
    if (comboHistory_.empty()) SelectNoCombo(enemy, availables);
    else SelectCombo(enemy, availables);

    //使用可能な技がないから終わり
    if (availables.empty()) return false;

    //priorityで昇順ソート
    std::sort(availables.begin(), availables.end(), [&](int a, int b) {
        return attacks_[a]->GetPriority() < attacks_[b]->GetPriority();
    });

    //一番優先度高い攻撃が何個あるか計算
    int selectSize = 0;
    int selectPrio = attacks_[availables.front()]->GetPriority();
    for (int i : availables) {
        if (attacks_[i]->GetPriority() == selectPrio) selectSize++;
        else break;
    }

    //ランダムで選択
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
        //使用可能の場合リストに追加
        if (attacks_.at(i)->CanUseAttack(e)) list.push_back(i);
    }
}

void SelectAttack::SelectCombo(EnemyBase* e, std::vector<int>& list)
{
    std::vector<int> combo = attacks_.at(currentAttack_)->GetComboList();
    for (int i = 0; i < attacks_.size(); i++) {
        //コンボ派生対象化どうか
        if (std::find(combo.begin(), combo.end(), i) == combo.end()) continue;

        //すでにコンボ攻撃しているかどうか
        if (std::find(comboHistory_.begin(), comboHistory_.end(), i) != comboHistory_.end()) continue;
       
        //攻撃できるなら追加
        if (attacks_.at(i)->CanUseAttack(e)) list.push_back(i);
    }

    //何も選べなかったらコンボリセットで
    if (list.empty()) ClearComboHistory();
}