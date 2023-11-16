#pragma once
#include "BehaviourNode.h"
#include <DirectXMath.h>

using namespace DirectX;
class EnemyBase;

class MoveTarget : public Action
{
	EnemyBase* owner_;
public:
	MoveTarget(EnemyBase* owner, float speed, float range);
	Status Update() override;
};