#pragma once
#include "BehaviourNode.h"
#include <string>

class EnemyBase;

//Enemyが設定したStateでないなら切り替える
class EnemyChangeStateNode : public Action
{
	std::string stateName_;
	EnemyBase* owner_;
public:
	EnemyChangeStateNode(EnemyBase* owner, std::string name);
	Status Update() override;
};

//Enemyが設定したStateでないなら切り替える
class EnemyChangeCombatStateNode : public Action
{
	std::string stateName_;
	EnemyBase* owner_;
public:
	EnemyChangeCombatStateNode(EnemyBase* owner, std::string name);
	Status Update() override;
};