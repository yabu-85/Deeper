#pragma once
#include <vector>
#include "../Engine/Singleton.h"
class EnemyBase;

class SelectAttackInfo : public Singleton<SelectAttackInfo> {
    int priority_;   //�����U���\���������̗D��x
public:
    SelectAttackInfo(int pri) : priority_(pri) {}
    virtual bool CanUseAttack(EnemyBase* enemy) { return false; }
    int GetPriority() { return priority_; }
    void SetPriority(int p) { priority_ = p; }

    // �������󂯎��GetInstance()���\�b�h
    static SelectAttackInfo& GetInstance(int pri) {
        static SelectAttackInfo instance(pri); // �C���X�^���X�����Ɉ�����n��
        return instance;
    }
};

class SelectoAttack {
    int selectAttack_;
    std::vector<SelectAttackInfo*> attacks_;
public:
    void Selector(EnemyBase* enemy);
    void AddSelectAttack(SelectAttackInfo* info);
    int GetSelectAttack() { return selectAttack_; }
};

class SwordBossSlashUp : public SelectAttackInfo {
    friend class Singleton<SwordBossSlashUp>;
public:
};