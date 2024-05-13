#include "EnemyAttackSelectNode.h"
#include "../Enemy/EnemyBase.h"
#include "../State/StateManager.h"
#include "../Engine/Global.h"

//�Ƃ��SwordBoss�ō쐬���Ă݂悤��
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
	bool select = boss->GetSelectoAttack()->Selector(boss);

	if(select) return Status::SUCCESS;
	return Status::FAILURE;
}
