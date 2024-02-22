#include "ChangeStateNode.h"
#include "../Enemy/EnemyBase.h"
#include "../State/StateManager.h"

EnemyChangeStateNode::EnemyChangeStateNode(EnemyBase* owner, std::string name)
	: Action(), stateName_(name), owner_(owner)
{
}

EnemyChangeStateNode::Status EnemyChangeStateNode::Update()
{
	if (owner_->GetCombatStateManager()->GetName() != stateName_)
		owner_->GetCombatStateManager()->ChangeState(stateName_);

	return Status::SUCCESS;
}

//----------------------------------------------------------------------------------------

EnemyChangeCombatStateNode::EnemyChangeCombatStateNode(EnemyBase* owner, std::string name)
	: Action(), stateName_(name), owner_(owner)
{
}

EnemyChangeCombatStateNode::Status EnemyChangeCombatStateNode::Update()
{
	if (owner_->GetCombatStateManager()->GetName() != stateName_)
		owner_->GetCombatStateManager()->ChangeState(stateName_);

	return Status::SUCCESS;
}

//----------------------------------------------------------------------------------------
