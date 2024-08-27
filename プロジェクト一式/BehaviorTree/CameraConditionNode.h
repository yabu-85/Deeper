#pragma once
#include "BehaviourNode.h"

class EnemyBase;

//�G�̌��_���X�N���[�����ɂ���Ȃ�Success��Ԃ�
class IsEnemyWithinScreen : public Condition
{
	EnemyBase* pEnemyBase_;
public:
	IsEnemyWithinScreen(TreeNode* child, EnemyBase* enemy);
	Status Update() override;
};