#pragma once
#include "BehaviourNode.h"
#include <string>

class EnemyBase;

//w’è‚µ‚½Enemy‚ªw’è‚µ‚½State‚Å‚È‚¢‚È‚ç‚»‚ÌState‚ÉØ‚è‘Ö‚¦‚é
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