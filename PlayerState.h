#pragma once
#include "StateBase.h"

class PlayerWait : public StateBase
{
public:
	PlayerWait(StateManager* owner) : StateBase(owner) {};
	const char* GetName() const override { return "Wait"; }
	void Update() override;
};

class PlayerWalk : public StateBase
{
public:
	PlayerWalk(StateManager* owner) : StateBase(owner) {};
	const char* GetName() const override { return "Walk"; }
	void Update() override;
};

class PlayerAvo : public StateBase
{
	int avoTime_;
public:
	PlayerAvo(StateManager* owner) : StateBase(owner), avoTime_(0) {};
	const char* GetName() const override { return "Avo"; }
	void Update() override;
	void OnEnter() override;
};

