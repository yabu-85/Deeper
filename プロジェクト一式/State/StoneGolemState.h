#pragma once
#include "StateBase.h"

class StateManager;
class Root;

class StoneGolemAppear : public StateBase
{
	int time_;
public:
	StoneGolemAppear(StateManager* owner);
	const char* GetName() const override { return "Appear"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class StoneGolemDead : public StateBase
{
	int time_;
public:
	StoneGolemDead(StateManager* owner);
	const char* GetName() const override { return "Dead"; }
	void Update() override;
	void OnEnter() override;
};

class StoneGolemPatrol : public StateBase
{
	int foundSearchTime_;
public:
	StoneGolemPatrol(StateManager* owner);
	const char* GetName() const override { return "Patrol"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class StoneGolemCombat : public StateBase
{
	int time_;
	Root* root_;
public:
	StoneGolemCombat(StateManager* owner);
	~StoneGolemCombat() override;
	const char* GetName() const override { return "Combat"; }
	void Update() override;
	void OnEnter() override;
};

//---------------------------------------------------------------------------------

class StoneGolemWait : public StateBase
{
	int time_;
public:
	StoneGolemWait(StateManager* owner);
	const char* GetName() const override { return "Wait"; }
	void Update() override;
	void OnEnter() override;
};

class StoneGolemMove : public StateBase
{
	int time_;
public:
	StoneGolemMove(StateManager* owner);
	const char* GetName() const override { return "Move"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class StoneGolemAttack : public StateBase
{
	int time_;
public:
	StoneGolemAttack(StateManager* owner);
	const char* GetName() const override { return "Attack"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};