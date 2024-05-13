#include "EnemyAttackSelect.h"
#include <algorithm>

#include "../Engine/GameObject.h"

SelectAttack::SelectAttack() : selectAttack_(0)
{
}

bool SelectAttack::Selector(EnemyBase* enemy)
{
    //使用な技を入れる
    std::vector<int> availables;
    for (int i = 0; i < attacks_.size(); i++) {
        //使用可能の場合リストに追加
        if (attacks_.at(i)->CanUseAttack(enemy)) availables.push_back(i);
    }

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
    selectAttack_ = availables[randSelect];
    return true;
}

void SelectAttack::AddSelectAttack(SelectAttackInfo* info)
{
    attacks_.push_back(info);
}