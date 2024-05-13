#pragma once
#include "EnemyAttackSelect.h"

class SwordBossSlashUp : public SelectAttackInfo, public Singleton<SwordBossSlashUp>{
    friend class Singleton<SwordBossSlashUp>;
public:
    SwordBossSlashUp();
    bool CanUseAttack(EnemyBase* enemy) override;
};

class SwordBossSlashRight : public SelectAttackInfo, public Singleton<SwordBossSlashUp> {
    friend class Singleton<SwordBossSlashRight>;
public:
    SwordBossSlashRight();
    bool CanUseAttack(EnemyBase* enemy) override;
};

class SwordBossThrust : public SelectAttackInfo, public Singleton<SwordBossThrust> {
    friend class Singleton<SwordBossThrust>;
public:
    SwordBossThrust();
    bool CanUseAttack(EnemyBase* enemy) override;
};