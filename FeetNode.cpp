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

IsNormalAttackState::IsNormalAttackState(TreeNode* child, Feet* enemy, bool flag) : Condition(child), owner_(enemy), isInAttack_(flag)
{
}

IsNormalAttackState::Status IsNormalAttackState::Update()
{
	if ((owner_->GetCombatStateManager()->GetName() == "Attack") == isInAttack_) {
		return child_->Tick();
	}

	return Status::FAILURE;
}
