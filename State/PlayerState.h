#pragma once
#include "StateBase.h"

class PlayerWait : public StateBase
{
public:
	PlayerWait(StateManager* owner);
	const char* GetName() const override { return "Wait"; }
	void Update() override;
	void OnEnter() override;
};

class PlayerWalk : public StateBase
{
public:
	PlayerWalk(StateManager* owner);
	const char* GetName() const override { return "Walk"; }
	void Update() override;
	void OnEnter() override;
};

class PlayerWeaponChange : public StateBase
{
	int time_;
public:
	PlayerWeaponChange(StateManager* owner);
	const char* GetName() const override { return "Change"; }
	void Update() override;
	void OnEnter() override;
};

class PlayerAvo : public StateBase
{
	int avoTime_;
	int nextCmd_;
public:
	PlayerAvo(StateManager* owner);
	const char* GetName() const override { return "Avo"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class PlayerAtk : public StateBase
{
	int time_;
	int nextCmd_;
public:
	PlayerAtk(StateManager* owner);
	const char* GetName() const override { return "Atk"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class PlayerSubAtk : public StateBase
{
	int nextCmd_;
public:
	PlayerSubAtk(StateManager* owner);
	const char* GetName() const override { return "SubAtk"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};