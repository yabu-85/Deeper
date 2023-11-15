#pragma once
#include "StateBase.h"

class Feet;
class StateManager;
class Root;

class FeetAppear : public StateBase
{
	Feet* pFeet_;
public:
	FeetAppear(StateManager* owner);
	const char* GetName() const override { return "Appear"; }
	void Update() override;
};

class FeetIdle : public StateBase
{
	Feet* pFeet_;
public:
	FeetIdle(StateManager* owner);
	const char* GetName() const override { return "Idle"; }
	void Update() override;
};

class FeetPatrol : public StateBase
{
	Feet* pFeet_;
public:
	FeetPatrol(StateManager* owner);
	const char* GetName() const override { return "Patrol"; }
	void Update() override;
};

class FeetCombat : public StateBase
{
	Feet* pFeet_;
	StateManager* pCombatStateManager_;
	Root* root_;
public:
	FeetCombat(StateManager* owner);
	~FeetCombat() override;
	const char* GetName() const override { return "Combat"; }
	void Update() override;
};

class FeetDead : public StateBase
{
	Feet* pFeet_;
public:
	FeetDead(StateManager* owner);
	const char* GetName() const override { return "Dead"; }
	void Update() override;
};

//---------------------------------------------------------------------------------

class FeetMove : public StateBase
{
	Feet* pFeet_;
public:
	FeetMove(StateManager* owner);
	const char* GetName() const override { return "Dead"; }
	void Update() override;
};