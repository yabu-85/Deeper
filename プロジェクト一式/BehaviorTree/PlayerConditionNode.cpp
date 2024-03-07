#include "PlayerConditionNode.h"
#include "../Player/Player.h"
#include "../Enemy/EnemyBase.h"

IsPlayerInRangeNode::IsPlayerInRangeNode(TreeNode* child, float range, EnemyBase* enemy, Player* player)
    : Condition(child), range_(range),pEnemyBase_(enemy), pPlayer_(player)
{
}

IsPlayerInRangeNode::Status IsPlayerInRangeNode::Update()
{
    XMFLOAT3 plaPos = pPlayer_->GetPosition();
    XMVECTOR vPlaPos = XMLoadFloat3(&plaPos);
    
    XMFLOAT3 pos = pEnemyBase_->GetPosition();
    XMVECTOR vPos = XMLoadFloat3(&pos);

    float length = XMVectorGetX(XMVector3Length(vPlaPos - vPos));
    if (length <= range_) {
        return child_->Tick();
    }

    return Status::FAILURE;
}

//--------------------------------------------------------------------------------------------

IsPlayerNotInRangeNode::IsPlayerNotInRangeNode(TreeNode* child, float range, EnemyBase* enemy, Player* player)
    : Condition(child), range_(range), pEnemyBase_(enemy), pPlayer_(player) 
{
}

IsPlayerNotInRangeNode::Status IsPlayerNotInRangeNode::Update()
{
    XMFLOAT3 plaPos = pPlayer_->GetPosition();
    XMVECTOR vPlaPos = XMLoadFloat3(&plaPos);

    XMFLOAT3 pos = pEnemyBase_->GetPosition();
    XMVECTOR vPos = XMLoadFloat3(&pos);

    float length = XMVectorGetX(XMVector3Length(vPlaPos - vPos));
    if (length >= range_) {
        return child_->Tick();
    }

    return Status::FAILURE;
}
