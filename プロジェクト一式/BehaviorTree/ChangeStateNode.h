#pragma once
#include "BehaviourNode.h"
#include <string>

class EnemyBase;

//Enemy‚ªİ’è‚µ‚½State‚Å‚È‚¢‚È‚çØ‚è‘Ö‚¦‚é
class EnemyChangeStateNode : public Action
{
	std::string stateName_;
	EnemyBase* owner_;
public:
	EnemyChangeStateNode(EnemyBase* owner, std::string name);
	Status Update() override;
};

//Enemy‚ªİ’è‚µ‚½State‚Å‚È‚¢‚È‚çØ‚è‘Ö‚¦‚é
class EnemyChangeCombatStateNode : public Action
{
	std::string stateName_;
	EnemyBase* owner_;
public:
	EnemyChangeCombatStateNode(EnemyBase* owner, std::string name);
	Status Update() override;
};