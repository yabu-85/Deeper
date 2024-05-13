#pragma once
#include <vector>
#include "../Engine/Singleton.h"
class EnemyBase;

class SelectAttackInfo {
    int priority_;   //ï°êîçUåÇâ¬î\ÇæÇ¡ÇΩéûÇÃóDêÊìx
public:
    SelectAttackInfo(int pri) : priority_(pri) {}
    virtual bool CanUseAttack(EnemyBase* enemy) { return false; }
    int GetPriority() { return priority_; }
    void SetPriority(int p) { priority_ = p; }
};

class SelectoAttack {
    int selectAttack_;
    std::vector<SelectAttackInfo*> attacks_;
public:
    SelectoAttack();
    bool Selector(EnemyBase* enemy);
    void AddSelectAttack(SelectAttackInfo* info);
    int GetSelectAttack() { return selectAttack_; }
};

class SwordBossSlashUp : public SelectAttackInfo, public Singleton<SwordBossSlashUp>{
    SwordBossSlashUp();
public:
    bool CanUseAttack(EnemyBase* enemy) override;
};

class SwordBossSlashRight : public SelectAttackInfo, public Singleton<SwordBossSlashRight> {
    SwordBossSlashRight();
public:
    bool CanUseAttack(EnemyBase* enemy) override;
};

class SwordBossThrust : public SelectAttackInfo, public Singleton<SwordBossThrust> {
    SwordBossThrust();
public:
    bool CanUseAttack(EnemyBase* enemy) override;
};