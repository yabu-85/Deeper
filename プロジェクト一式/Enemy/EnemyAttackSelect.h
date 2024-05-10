#include <vector>
#include <algorithm>
class EnemyBase;

//�܂��g�p�\�܂ō쐬�o���Ă��Ȃ����
//�U����o�^���āA���̒�����U����I�ԃN���X

//Singleton���g�p���邩�ǂ��������ǂ���
//�����g�p�������������Ǝv������Engine�ɂ���
//Singleton���܂��ł��Ă��Ȃ�

class SelectAttackInfo {
    int priority_;   //�����U���\���������̗D��x
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