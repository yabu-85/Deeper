#pragma once
#include <vector>
#include "../Engine/Singleton.h"
class EnemyBase;

class SelectAttackInfo
{
    int priority_;   //複数攻撃可能だった時の優先度
public:
    SelectAttackInfo() : priority_(0) {}
    virtual bool CanUseAttack(EnemyBase* enemy) { return false; }
    int GetPriority() { return priority_; }
    void SetPriority(int p) { priority_ = p; }
};

class SelectAttack {
    int selectAttack_;
    std::vector<SelectAttackInfo*> attacks_;
public:
    SelectAttack();
    bool Selector(EnemyBase* enemy);
    void AddSelectAttack(SelectAttackInfo* info);
    int GetSelectAttack() { return selectAttack_; }
};