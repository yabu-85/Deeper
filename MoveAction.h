#pragma once
#include "BehaviourNode.h"
#include <DirectXMath.h>

using namespace DirectX;
class EnemyBase;

class MoveTarget : public Action
{
	float speed_;		//�ړ��X�s�[�h
	float range_;		//�ړ��I�������
	EnemyBase* owner_;
public:
	MoveTarget(EnemyBase* owner, float speed, float range);
	Status Update() override;
};