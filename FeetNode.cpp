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