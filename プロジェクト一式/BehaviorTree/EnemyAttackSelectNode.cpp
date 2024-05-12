#include "EnemyAttackSelectNode.h"
#include "../Enemy/EnemyBase.h"
#include "../State/StateManager.h"
#include "../Engine/Global.h"

//Ç∆ÇËÇ‹SwordBossÇ≈çÏê¨ÇµÇƒÇ›ÇÊÇ§Ç‚
#include "../GameManager/GameManager.h"
#include "../Enemy/SwordBoss.h"
#include "../Player/Player.h"

EnemyAttackSelectNode::EnemyAttackSelectNode(EnemyBase* owner) : Action(), owner_(owner)
{
}

EnemyAttackSelectNode::Status EnemyAttackSelectNode::Update()
{
	SwordBoss* boss = static_cast<SwordBoss*>(owner_);

	if (rand() % 2 == 0) {
		DamageInfo damage(boss, "SwordBossAtk1", 0);
		boss->GetDamageController()->SetCurrentDamage(damage);
	}
	else {
		DamageInfo damage(boss, "none", 0);
		boss->GetDamageController()->SetCurrentDamage(damage);
	}

	return Status::SUCCESS;
}
