#pragma once
#include "StateBase.h"
class Player;

class PlayerWait : public StateBase
{
	Player* pPlayer_;
public:
	PlayerWait(StateManager* owner);
	const char* GetName() const override { return "Wait"; }
	void Update() override;
};

class PlayerWalk : public StateBase
{
	Player* pPlayer_;
public:
	PlayerWalk(StateManager* owner);
	const char* GetName() const override { return "Walk"; }
	void Update() override;
};

class PlayerWeaponChange : public StateBase
{
	int time_;
	int changeTime_;
	Player* pPlayer_;
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
	Player* pPlayer_;
public:
	PlayerAvo(StateManager* owner);
	const char* GetName() const override { return "Avo"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class PlayerAtk : public StateBase
{
	int nextCmd_;
	Player* pPlayer_;
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
	Player* pPlayer_;
public:
	PlayerSubAtk(StateManager* owner);
	const char* GetName() const override { return "SubAtk"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

class PlayerDead : public StateBase
{
	Player* pPlayer_;
public:
	PlayerDead(StateManager* owner);
	const char* GetName() const override { return "Dead"; }
	void Update() override;
	void OnEnter() override;
};