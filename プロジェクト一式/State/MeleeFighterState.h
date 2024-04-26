#pragma once
#include "StateBase.h"

class StateManager;
class Root;

class MeleeFighterAppear : public StateBase
{
	int time_;
public:
	MeleeFighterAppear(StateManager* owner);
	const char* GetName() const override { return "Appear"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class MeleeFighterDead : public StateBase
{
	int time_;
public:
	MeleeFighterDead(StateManager* owner);
	const char* GetName() const override { return "Dead"; }
	void Update() override;
	void OnEnter() override;
};

class MeleeFighterPatrol : public StateBase
{
	int foundSearchTime_;
public:
	MeleeFighterPatrol(StateManager* owner);
	const char* GetName() const override { return "Patrol"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class MeleeFighterCombat : public StateBase
{
	int time_;
	Root* root_;
public:
	MeleeFighterCombat(StateManager* owner);
	~MeleeFighterCombat() override;
	const char* GetName() const override { return "Combat"; }
	void Update() override;
	void OnEnter() override;
};

//---------------------------------------------------------------------------------

class MeleeFighterWait : public StateBase
{
public:
	MeleeFighterWait(StateManager* owner);
	const char* GetName() const override { return "Wait"; }
	void Update() override;
	void OnEnter() override;
};

class MeleeFighterMove : public StateBase
{
	int time_;
public:
	MeleeFighterMove(StateManager* owner);
	const char* GetName() const override { return "Move"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class MeleeFighterAttack : public StateBase
{
	int time_;
public:
	MeleeFighterAttack(StateManager* owner);
	const char* GetName() const override { return "Attack"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};