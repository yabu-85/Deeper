#include "EnemyAttackSelectNode.h"
#include "../Enemy/EnemyBase.h"
#include "../State/StateManager.h"
#include "../Engine/Global.h"

//Ç∆ÇËÇ‹SwordBossÇ≈çÏê¨ÇµÇƒÇ›ÇÊÇ§Ç‚
#include "../GameManager/GameManager.h"
#include "../Enemy/SwordBoss.h"
#include "../Player/Player.h"
#include "../Enemy/EnemyAttackSelect.h"

EnemyAttackSelectNode::EnemyAttackSelectNode(EnemyBase* owner) : Action(), owner_(owner)
{
}

EnemyAttackSelectNode::Status EnemyAttackSelectNode::Update()
{
	SwordBoss* boss = static_cast<SwordBoss*>(owner_);
	bool select = boss->GetSelectAttack()->Selector(boss);

	if(select) return Status::SUCCESS;
	return Status::FAILURE;
}
