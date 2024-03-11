#include "CameraConditionNode.h"
#include "../Enemy/EnemyBase.h"
#include "../Engine/Camera.h"

IsEnemyWithinScreen::IsEnemyWithinScreen(TreeNode* child, EnemyBase* enemy) : Condition(child), pEnemyBase_(enemy)
{
}

IsEnemyWithinScreen::Status IsEnemyWithinScreen::Update()
{
    XMFLOAT3 pos = pEnemyBase_->GetPosition();
    float height = pEnemyBase_->GetAimTargetPos();
    pos.y += height;
    if (Camera::IsPositionWithinScreen(pos, 1.0f)) {
        return child_->Tick();
    }

    return Status::FAILURE;
}
