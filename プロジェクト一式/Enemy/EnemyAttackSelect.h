#include <vector>
#include <algorithm>
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
    void Selector(EnemyBase* enemy);
    void AddSelectAttack(SelectAttackInfo* info);
    int GetSelectAttack() { return selectAttack_; }
};

//---------------------------------------------------

class MelleAttack1 : public SelectAttackInfo {
public:
    MelleAttack1(int pri) : SelectAttackInfo(pri) {}
    bool CanUseAttack(EnemyBase* enemy) override;
};

class MelleAttack2 : public SelectAttackInfo {
public:
    MelleAttack2(int pri) : SelectAttackInfo(pri) {}
    bool CanUseAttack(EnemyBase* enemy) override;
};