#pragma once
#include "StateBase.h"

class StateManager;
class Root;

class FeetAppear : public StateBase
{
	int time_;
public:
	FeetAppear(StateManager* owner);
	const char* GetName() const override { return "Appear"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class FeetIdle : public StateBase
{
public:
	FeetIdle(StateManager* owner);
	const char* GetName() const override { return "Idle"; }
	void Update() override;
};

class FeetPatrol : public StateBase
{
	int foundSearchTime_;
public:
	FeetPatrol(StateManager* owner);
	const char* GetName() const override { return "Patrol"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class FeetCombat : public StateBase
{
	Root* root_;
public:
	FeetCombat(StateManager* owner);
	~FeetCombat() override;
	const char* GetName() const override { return "Combat"; }
	void Update() override;
	void OnEnter() override;
};

class FeetDead : public StateBase
{
public:
	FeetDead(StateManager* owner);
	const char* GetName() const override { return "Dead"; }
	void Update() override;
};

//---------------------------------------------------------------------------------

class FeetWait : public StateBase
{
public:
	FeetWait(StateManager* owner);
	const char* GetName() const override { return "Wait"; }
	void Update() override;
	void OnEnter() override;
};

class FeetMove : public StateBase
{
public:
	FeetMove(StateManager* owner);
	const char* GetName() const override { return "Move"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class FeetAttack : public StateBase
{
	int time_;
public:
	FeetAttack(StateManager* owner);
	const char* GetName() const override { return "Attack"; }
	void Update() override;
	void OnEnter() override;
};