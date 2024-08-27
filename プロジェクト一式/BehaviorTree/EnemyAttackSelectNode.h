#pragma once
#include "BehaviourNode.h"
#include <string>

class EnemyBase;
class SwordBoss;

//AttackSelector�ŋZ���I���\��������Success��Ԃ�
class SwordBossAttackSelectNode : public Action
{
	SwordBoss* owner_;
public:
	SwordBossAttackSelectNode(EnemyBase* owner);
	Status Update() override;
};