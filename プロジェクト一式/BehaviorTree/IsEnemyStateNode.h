#pragma once
#include <string>
#include "BehaviourNode.h"

class EnemyBase;

//�w�肵���X�e�[�g�Ȃ�q����Update
class IsEnemyState : public Condition
{
	std::string stateName_;
	EnemyBase* pEnemyBase_;
public:
	IsEnemyState(TreeNode* child, std::string name, EnemyBase* enemy);
	Status Update() override;
};

//�w�肵���X�e�[�g�łȂ��Ȃ�q����Update
class IsNotEnemyState : public Condition
{
	std::string stateName_;
	EnemyBase* pEnemyBase_;
public:
	IsNotEnemyState(TreeNode* child, std::string name, EnemyBase* enemy);
	Status Update() override;
};

//--------------------------------------------------------------------------------------------

//�w�肵���X�e�[�g�Ȃ�q����Update
class IsEnemyCombatState : public Condition
{
	std::string stateName_;
	EnemyBase* pEnemyBase_;
public:
	IsEnemyCombatState(TreeNode* child, std::string name, EnemyBase* enemy);
	Status Update() override;
};

//�w�肵���X�e�[�g�łȂ��Ȃ�q����Update
class IsNotEnemyCombatState : public Condition
{
	std::string stateName_;
	EnemyBase* pEnemyBase_;
public:
	IsNotEnemyCombatState(TreeNode* child, std::string name, EnemyBase* enemy);
	Status Update() override;
};

//--------------------------------------------------------------------------------------------
