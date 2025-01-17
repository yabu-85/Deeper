#pragma once
#include "BehaviourNode.h"
#include <string>

class EnemyBase;
class SwordBoss;

//AttackSelectorで技が選択可能だったらSuccessを返す
class SwordBossAttackSelectNode : public Action
{
	SwordBoss* owner_;
public:
	SwordBossAttackSelectNode(EnemyBase* owner);
	Status Update() override;
};