#pragma once
#include "BehaviourNode.h"
#include <DirectXMath.h>

using namespace DirectX;
class EnemyBase;

class MoveTarget : public Action
{
	float speed_;		//移動スピード
	float range_;		//移動終了判定の
	EnemyBase* owner_;
public:
	MoveTarget(EnemyBase* owner, float speed, float range);
	Status Update() override;
};