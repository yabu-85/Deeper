#include "PlayerConditionNode.h"
#include "../Player/Player.h"
#include "../Enemy/EnemyBase.h"
#include "../Engine/Global.h"

IsPlayerInRangeNode::IsPlayerInRangeNode(TreeNode* child, float range, EnemyBase* enemy, Player* player)
    : Condition(child), range_(range),pEnemyBase_(enemy), pPlayer_(player)
{
}

IsPlayerInRangeNode::Status IsPlayerInRangeNode::Update()
{
    float length = DistanceCalculation(pPlayer_->GetPosition(), pEnemyBase_->GetPosition());
    if (length <= range_) return child_->Tick();
    return Status::FAILURE;
}

//--------------------------------------------------------------------------------------------

IsPlayerNotInRangeNode::IsPlayerNotInRangeNode(TreeNode* child, float range, EnemyBase* enemy, Player* player)
    : Condition(child), range_(range), pEnemyBase_(enemy), pPlayer_(player) 
{
}

IsPlayerNotInRangeNode::Status IsPlayerNotInRangeNode::Update()
{
    float length = DistanceCalculation(pPlayer_->GetPosition(), pEnemyBase_->GetPosition());
    if (length >= range_) return child_->Tick();
    return Status::FAILURE;
}