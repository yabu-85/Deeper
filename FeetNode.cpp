#include "FeetNode.h"
#include "Feet.h"
#include "StateManager.h"

FeetNormalAttack::FeetNormalAttack(Feet* owner) : Action(), owner_(owner) {}

FeetNormalAttack::Status FeetNormalAttack::Update()
{
    owner_->GetStateManager()->ChangeState("Combat");
    owner_->GetCombatStateManager()->ChangeState("Attack");

    return Status::SUCCESS;
}

//-------------------------------------------------------------------------------------------------

IsNormalAttackState::IsNormalAttackState(TreeNode* child, Feet* enemy) : Condition(child), owner_(enemy)
{
}

IsNormalAttackState::Status IsNormalAttackState::Update()
{
	std::string name = owner_->GetCombatStateManager()->GetName();
	if (name != "Attack") {
		return child_->Tick();
	}

	return Status::FAILURE;
}
