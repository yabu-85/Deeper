#include "SwordBossAttacks.h"
#include "SwordBoss.h"
#include "../GameManager/GameManager.h"
#include "../Engine/Global.h"
#include "../Player/Player.h"

namespace {
    static const int PRIORITY_SLASH_UP = 0;
    static const int PRIORITY_SLASH_RIGHT = 0;
    static const int PRIORITY_THRUST = 0;

    static const float SLASH_UP_DIST = 2.0f;
    static const float SLASH_RIGHT_DIST = 2.0f;
    static const float THRUST_DIST = 3.5f;

}

SwordBossSlashUp::SwordBossSlashUp() { SetPriority(PRIORITY_SLASH_UP); }
bool SwordBossSlashUp::CanUseAttack(EnemyBase* enemy)
{
    float dist = CalculationDistance(GameManager::GetPlayer()->GetPosition(), enemy->GetPosition());
    return (dist <= SLASH_UP_DIST);
}

//------------------------------------------------------------------------------------------

SwordBossSlashRight::SwordBossSlashRight() { SetPriority(PRIORITY_SLASH_RIGHT); }
bool SwordBossSlashRight::CanUseAttack(EnemyBase* enemy)
{
    float dist = CalculationDistance(GameManager::GetPlayer()->GetPosition(), enemy->GetPosition());
    return (dist <= SLASH_RIGHT_DIST);
}

//------------------------------------------------------------------------------------------

SwordBossThrust::SwordBossThrust() { SetPriority(PRIORITY_THRUST); }
bool SwordBossThrust::CanUseAttack(EnemyBase* enemy)
{
    float dist = CalculationDistance(GameManager::GetPlayer()->GetPosition(), enemy->GetPosition());
    return (dist <= THRUST_DIST);
}
