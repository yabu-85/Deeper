#pragma once
#include <string>
#include "BehaviourNode.h"

class EnemyBase;

class IsEnemyState : public Condition
{
	std::string stateName_;
	EnemyBase* pEnemyBase_;
public:
	IsEnemyState(TreeNode* child, std::string name, EnemyBase* enemy);
	Status Update() override;
};

class IsNotEnemyState : public Condition
{
	std::string stateName_;
	EnemyBase* pEnemyBase_;
public:
	IsNotEnemyState(TreeNode* child, std::string name, EnemyBase* enemy);
	Status Update() override;
};

//--------------------------------------------------------------------------------------------

class IsEnemyCombatState : public Condition
{
	std::string stateName_;
	EnemyBase* pEnemyBase_;
public:
	IsEnemyCombatState(TreeNode* child, std::string name, EnemyBase* enemy);
	Status Update() override;
};

class IsNotEnemyCombatState : public Condition
{
	std::string stateName_;
	EnemyBase* pEnemyBase_;
public:
	IsNotEnemyCombatState(TreeNode* child, std::string name, EnemyBase* enemy);
	Status Update() override;
};

//--------------------------------------------------------------------------------------------
