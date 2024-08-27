#pragma once
#include "BehaviourNode.h"
#include <string>

class EnemyBase;

//Enemy���ݒ肵��State�łȂ��Ȃ�؂�ւ���
class EnemyChangeStateNode : public Action
{
	std::string stateName_;
	EnemyBase* owner_;
public:
	EnemyChangeStateNode(EnemyBase* owner, std::string name);
	Status Update() override;
};

//Enemy���ݒ肵��State�łȂ��Ȃ�؂�ւ���
class EnemyChangeCombatStateNode : public Action
{
	std::string stateName_;
	EnemyBase* owner_;
public:
	EnemyChangeCombatStateNode(EnemyBase* owner, std::string name);
	Status Update() override;
};