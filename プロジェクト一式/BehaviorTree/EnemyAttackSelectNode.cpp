#include "EnemyAttackSelectNode.h"
#include "../Enemy/EnemyBase.h"
#include "../State/StateManager.h"
#include "../Engine/Global.h"

//�Ƃ��SwordBoss�ō쐬���Ă݂悤��
#include "../GameManager/GameManager.h"
#include "../Enemy/SwordBoss.h"
#include "../Player/Player.h"
#include "../Enemy/EnemyAttackSelect.h"

SwordBossAttackSelectNode::SwordBossAttackSelectNode(EnemyBase* owner) : Action()
{
	owner_ = static_cast<SwordBoss*>(owner);
}

SwordBossAttackSelectNode::Status SwordBossAttackSelectNode::Update()
{
	if(owner_->GetSelectAttack()->Selector(owner_)) return Status::SUCCESS;
	return Status::FAILURE;
}