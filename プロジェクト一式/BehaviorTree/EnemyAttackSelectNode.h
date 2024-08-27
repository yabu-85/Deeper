#pragma once
#include "BehaviourNode.h"
#include <string>

class EnemyBase;
class SwordBoss;

//AttackSelector‚Å‹Z‚ª‘I‘ð‰Â”\‚¾‚Á‚½‚çSuccess‚ð•Ô‚·
class SwordBossAttackSelectNode : public Action
{
	SwordBoss* owner_;
public:
	SwordBossAttackSelectNode(EnemyBase* owner);
	Status Update() override;
};