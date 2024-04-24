#pragma once
#include "StateBase.h"

class Root;
class SwordBoss;

class SwordBossAppear : public StateBase
{
	int time_;
public:
	SwordBossAppear(StateManager* owner);
	const char* GetName() const override { return "Appear"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class SwordBossDead : public StateBase
{
	int time_;
public:
	SwordBossDead(StateManager* owner);
	const char* GetName() const override { return "Dead"; }
	void Update() override;
	void OnEnter() override;
};

class SwordBossCombat : public StateBase
{
	unsigned time_;
	Root* root_;
public:
	SwordBossCombat(StateManager* owner);
	~SwordBossCombat() override;
	const char* GetName() const override { return "Combat"; }
	void Update() override;
	void OnEnter() override;
};

//---------------------------------------------------------------------------------

class SwordBossWait : public StateBase
{
public:
	SwordBossWait(StateManager* owner);
	const char* GetName() const override { return "Wait"; }
	void Update() override;
	void OnEnter() override;
};

class SwordBossMove : public StateBase
{
	int time_;
public:
	SwordBossMove(StateManager* owner);
	const char* GetName() const override { return "Move"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

//--------------------������SwordBoss�������ɏ���
#include "../Enemy/EnemyAttackBase.h"

class SwordBossAttack1 : public EnemyFrame {
	SwordBoss* pBoss_;
public:
	SwordBossAttack1(int start, int end);
	void Update() override;
	void OnExit() override;
};

class SwordBossAttack : public StateBase
{
	int time_;
	SwordBoss* pBoss_;
	EnemyAttackController* pAttackController_;

public:
	SwordBossAttack(StateManager* owner);
	const char* GetName() const override { return "Attack"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};
