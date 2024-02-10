#pragma once
#include "StateBase.h"

class StateManager;
class Root;

class ThrowAppear : public StateBase
{
	int time_;
public:
	ThrowAppear(StateManager* owner);
	const char* GetName() const override { return "Appear"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class ThrowPatrol : public StateBase
{
	int foundSearchTime_;
public:
	ThrowPatrol(StateManager* owner);
	const char* GetName() const override { return "Patrol"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class ThrowCombat : public StateBase
{
	Root* root_;
public:
	ThrowCombat(StateManager* owner);
	~ThrowCombat() override;
	const char* GetName() const override { return "Combat"; }
	void Update() override;
	void OnEnter() override;
};

class ThrowDead : public StateBase
{
public:
	ThrowDead(StateManager* owner);
	const char* GetName() const override { return "Dead"; }
	void Update() override;
};

//---------------------------------------------------------------------------------

class ThrowWait : public StateBase
{
public:
	ThrowWait(StateManager* owner);
	const char* GetName() const override { return "Wait"; }
	void Update() override;
	void OnEnter() override;
};

class ThrowMove : public StateBase
{
public:
	ThrowMove(StateManager* owner);
	const char* GetName() const override { return "Move"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class ThrowAttack : public StateBase
{
	int time_;
public:
	ThrowAttack(StateManager* owner);
	const char* GetName() const override { return "Attack"; }
	void Update() override;
	void OnEnter() override;
};