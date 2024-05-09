#include <vector>
#include <algorithm>
class EnemyBase;

class SelectAttackInfo {
    bool valid_;     //�g�p�\��
    int priority_;  //�����U���\���������̗D��x
public:
    SelectAttackInfo(int pri) : priority_(pri), valid_(true) {}
    virtual bool CanUseAttack(EnemyBase* enemy) { return false; }
    
    int GetPriority() { return priority_; }
    void SetPriority(int p) { priority_ = p; }
    void SetValid(bool b) { valid_ = b; }
    bool GetValid() { return valid_; }
};

class SelectoAttack {
    int selectAttack_;
    std::vector<SelectAttackInfo> attacks_;
public:
    void Selector(EnemyBase* enemy);
    void AddSelectAttack(SelectAttackInfo& info);
    int GetSelectAttack() { return selectAttack_; }
};