#pragma once
#include "StateBase.h"

class Root;
class SwordBoss;
class EnemyBase;
enum class SWORDBOSS_ANIMATION;

class SwordBossAppear : public StateBase
{
	int time_;
public:
	SwordBossAppear(StateManager* owner) : StateBase(owner), time_(0) {};
	const char* GetName() const override { return "Appear"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class SwordBossDead : public StateBase
{
	int time_;
public:
	SwordBossDead(StateManager* owner) : StateBase(owner), time_(0) {};
	const char* GetName() const override { return "Dead"; }
	void Update() override;
	void OnEnter() override;
};

class SwordBossCombat : public StateBase
{
	int time_;
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
	SwordBossWait(StateManager* owner) : StateBase(owner) {};
	const char* GetName() const override { return "Wait"; }
	void Update() override;
	void OnEnter() override;
};

class SwordBossMove : public StateBase
{
	int time_;
public:
	SwordBossMove(StateManager* owner) : StateBase(owner), time_(0) {};
	const char* GetName() const override { return "Move"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class SwordBossAttack : public StateBase
{
	int time_;
	SWORDBOSS_ANIMATION nextAttack_;
	SwordBoss* pBoss_;

	void UpdateSlashUp();
	void UpdateSlashRight();
	void UpdateSlashJump();
	void UpdateThrust();
public:
	SwordBossAttack(StateManager* owner);
	const char* GetName() const override { return "Attack"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};
