#pragma once
#include "BehaviourNode.h"
#include <string>

class EnemyBase;

//�w�肵��Enemy���w�肵��State�łȂ��Ȃ炻��State�ɐ؂�ւ���
class EnemyChangeStateNode : public Action
{
	std::string stateName_;
	EnemyBase* owner_;
public:
	EnemyChangeStateNode(EnemyBase* owner, std::string name);
	Status Update() override;
};

class EnemyChangeCombatStateNode : public Action
{
	std::string stateName_;
	EnemyBase* owner_;
public:
	EnemyChangeCombatStateNode(EnemyBase* owner, std::string name);
	Status Update() override;
};