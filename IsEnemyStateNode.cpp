#include "IsEnemyStateNode.h"
#include "EnemyBase.h"
#include "StateManager.h"

IsEnemyState::IsEnemyState(TreeNode* child, std::string name, EnemyBase* enemy)
	: Condition(child), pEnemyBase_(enemy), stateName_(name)
{
}

IsEnemyState::Status IsEnemyState::Update()
{
	if (pEnemyBase_->GetStateManager()->GetName() == stateName_) {
		return child_->Tick();
	}

	return Status::FAILURE;
}

//--------------------------------------------------------------------------------------------

IsNotEnemyState::IsNotEnemyState(TreeNode* child, std::string name, EnemyBase* enemy)
	: Condition(child), pEnemyBase_(enemy), stateName_(name)
{
}

IsNotEnemyState::Status IsNotEnemyState::Update()
{
	if (pEnemyBase_->GetStateManager()->GetName() != stateName_) {
		return child_->Tick();
	}

	return Status::FAILURE;
}

//--------------------------------------------------------------------------------------------

IsEnemyCombatState::IsEnemyCombatState(TreeNode* child, std::string name, EnemyBase* enemy)
	: Condition(child), pEnemyBase_(enemy), stateName_(name)
{
}

IsEnemyCombatState::Status IsEnemyCombatState::Update()
{
	if (pEnemyBase_->GetCombatStateManager()->GetName() == stateName_) {
		return child_->Tick();
	}

	return Status::FAILURE;
}

//--------------------------------------------------------------------------------------------

IsNotEnemyCombatState::IsNotEnemyCombatState(TreeNode* child, std::string name, EnemyBase* enemy)
	: Condition(child), pEnemyBase_(enemy), stateName_(name)
{
}

IsNotEnemyCombatState::Status IsNotEnemyCombatState::Update()
{
	if (pEnemyBase_->GetCombatStateManager()->GetName() != stateName_) {
		return child_->Tick();
	}

	return Status::FAILURE;
}

//--------------------------------------------------------------------------------------------
