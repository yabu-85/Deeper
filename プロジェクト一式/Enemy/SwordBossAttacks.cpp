#include "SwordBossAttacks.h"
#include "SwordBoss.h"
#include "../GameManager/GameManager.h"
#include "../Engine/Global.h"
#include "../Player/Player.h"

namespace {
    //優先度
    static const int PRIORITY_SLASH_UP = 0;
    static const int PRIORITY_SLASH_RIGHT = 0;
    static const int PRIORITY_SLASH_JUMP = 0;
    static const int PRIORITY_THRUST = 0;

    //距離
    static const float SLASH_UP_DIST = 2.5f;
    static const float SLASH_RIGHT_DIST = 2.5f;
    static const float SLASH_JUMP_DIST[2] = { 3.5f, 6.0f }; //1以上 / 2以下
    static const float THRUST_DIST = 3.5f;

    //コンボ推移表
    static const int SLASH_UP_COMBO[] = { SWORDBOSS_ATTACK_TYPE::Slash_Right, SWORDBOSS_ATTACK_TYPE::Thrust };
    static const int SLASH_RIGHT_COMBO[] = { SWORDBOSS_ATTACK_TYPE::Slash_Up, SWORDBOSS_ATTACK_TYPE::Thrust };
    static const int SLASH_JUMP_COMBO[] = { SWORDBOSS_ATTACK_TYPE::Slash_Right, SWORDBOSS_ATTACK_TYPE::Slash_Up, SWORDBOSS_ATTACK_TYPE::Thrust };
    static const int THRUST_COMBO[] = { SWORDBOSS_ATTACK_TYPE::Slash_Right, SWORDBOSS_ATTACK_TYPE::Slash_Up };

}

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

SwordBossSlashUp::SwordBossSlashUp() {
    for (int i : SLASH_UP_COMBO) AddComboList(i);
    SetPriority(PRIORITY_SLASH_UP); 
}

bool SwordBossSlashUp::CanUseAttack(EnemyBase* enemy)
{
    float dist = CalculationDistance(GameManager::GetPlayer()->GetPosition(), enemy->GetPosition());
    return (dist <= SLASH_UP_DIST);
}

//------------------------------------------------------------------------------------------

SwordBossSlashRight::SwordBossSlashRight() { 
    for (int i : SLASH_RIGHT_COMBO) AddComboList(i);
    SetPriority(PRIORITY_SLASH_RIGHT);
}

bool SwordBossSlashRight::CanUseAttack(EnemyBase* enemy)
{
    float dist = CalculationDistance(GameManager::GetPlayer()->GetPosition(), enemy->GetPosition());
    return (dist <= SLASH_RIGHT_DIST);
}

//------------------------------------------------------------------------------------------

SwordBossSlashJump::SwordBossSlashJump() {
    for (int i : SLASH_JUMP_COMBO) AddComboList(i);
    SetPriority(PRIORITY_SLASH_JUMP);
}

bool SwordBossSlashJump::CanUseAttack(EnemyBase* enemy)
{
    float dist = CalculationDistance(GameManager::GetPlayer()->GetPosition(), enemy->GetPosition());
    return (dist >= SLASH_JUMP_DIST[0] && dist <= SLASH_JUMP_DIST[1]);
}

//------------------------------------------------------------------------------------------

SwordBossThrust::SwordBossThrust() {
    for (int i : THRUST_COMBO) AddComboList(i);
    SetPriority(PRIORITY_THRUST);
}

bool SwordBossThrust::CanUseAttack(EnemyBase* enemy)
{
    float dist = CalculationDistance(GameManager::GetPlayer()->GetPosition(), enemy->GetPosition());
    return (dist <= THRUST_DIST);
}
