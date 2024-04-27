#pragma once
#include "StateBase.h"

class StateManager;
class Root;

class ThrowAppear : public StateBase
{
	int time_;
public:
	ThrowAppear(StateManager* owner) : StateBase(owner), time_(0) {};
	const char* GetName() const override { return "Appear"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class ThrowDead : public StateBase
{
	int time_;
public:
	ThrowDead(StateManager* owner) : StateBase(owner), time_(0) {};
	const char* GetName() const override { return "Dead"; }
	void Update() override;
	void OnEnter() override;
};

class ThrowPatrol : public StateBase
{
	int time_;
public:
	ThrowPatrol(StateManager* owner) : StateBase(owner), time_(0) {};
	const char* GetName() const override { return "Patrol"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class ThrowCombat : public StateBase
{
	int time_;
	Root* root_;
public:
	ThrowCombat(StateManager* owner);
	~ThrowCombat() override;
	const char* GetName() const override { return "Combat"; }
	void Update() override;
	void OnEnter() override;
};

//---------------------------------------------------------------------------------

class ThrowWait : public StateBase
{
public:
	ThrowWait(StateManager* owner) : StateBase(owner) {};
	const char* GetName() const override { return "Wait"; }
	void Update() override;
	void OnEnter() override;
};

class ThrowMove : public StateBase
{
public:
	ThrowMove(StateManager* owner) : StateBase(owner) {};
	const char* GetName() const override { return "Move"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class ThrowAttack : public StateBase
{
	int time_;
public:
	ThrowAttack(StateManager* owner) : StateBase(owner), time_(0) {};
	const char* GetName() const override { return "Attack"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};