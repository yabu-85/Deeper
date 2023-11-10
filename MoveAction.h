#pragma once
#include "BehaviourNode.h"
#include <DirectXMath.h>

using namespace DirectX;
class GameObject;

class MoveAction : public Action
{
	float speed_;
	GameObject* owner_;
public:
	MoveAction(std::string name, GameObject* owner);
	Status Update() override;
	void SetSpeed(float speed) { speed_ = speed; }
};

