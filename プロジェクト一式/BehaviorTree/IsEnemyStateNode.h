#pragma once
#include <string>
#include "BehaviourNode.h"

class EnemyBase;

//指定したステートなら子供をUpdate
class IsEnemyState : public Condition
{
	std::string stateName_;
	EnemyBase* pEnemyBase_;
public:
	IsEnemyState(TreeNode* child, std::string name, EnemyBase* enemy);
	Status Update() override;
};

//指定したステートでないなら子供をUpdate
class IsNotEnemyState : public Condition
{
	std::string stateName_;
	EnemyBase* pEnemyBase_;
public:
	IsNotEnemyState(TreeNode* child, std::string name, EnemyBase* enemy);
	Status Update() override;
};

//--------------------------------------------------------------------------------------------

//指定したステートなら子供をUpdate
class IsEnemyCombatState : public Condition
{
	std::string stateName_;
	EnemyBase* pEnemyBase_;
public:
	IsEnemyCombatState(TreeNode* child, std::string name, EnemyBase* enemy);
	Status Update() override;
};

//指定したステートでないなら子供をUpdate
class IsNotEnemyCombatState : public Condition
{
	std::string stateName_;
	EnemyBase* pEnemyBase_;
public:
	IsNotEnemyCombatState(TreeNode* child, std::string name, EnemyBase* enemy);
	Status Update() override;
};

//--------------------------------------------------------------------------------------------
