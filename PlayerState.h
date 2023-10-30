#pragma once
#include "StateBase.h"
class Player;
class DamageCtrl;

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

class PlayerAvo : public StateBase
{
	int avoTime_;
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
	int atkTime_;
	Player* pPlayer_;
	DamageCtrl* pDamageCtrl_;
public:
	PlayerAtk(StateManager* owner);
	const char* GetName() const override { return "Atk"; }
	void Update() override;
	void OnEnter() override;
};

class PlayerSubAtk : public StateBase
{
	int atkTime_;
	Player* pPlayer_;
	DamageCtrl* pDamageCtrl_;
public:
	PlayerSubAtk(StateManager* owner);
	const char* GetName() const override { return "Atk"; }
	void Update() override;
	void OnEnter() override;
};