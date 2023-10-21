#pragma once
#include "StateBase.h"
class Player;

class PlayerWait : public StateBase
{
	Player* pPlayer_;
public:
	PlayerWait(StateManager* owner) : StateBase(owner), pPlayer_(nullptr) {};
	const char* GetName() const override { return "Wait"; }
	void Update() override;
	void Initialize() override;
};

class PlayerWalk : public StateBase
{
	Player* pPlayer_;
public:
	PlayerWalk(StateManager* owner) : StateBase(owner), pPlayer_(nullptr) {};
	const char* GetName() const override { return "Walk"; }
	void Update() override;
	void Initialize() override;
};

class PlayerAvo : public StateBase
{
	int avoTime_;
	Player* pPlayer_;
public:
	PlayerAvo(StateManager* owner) : StateBase(owner), avoTime_(0), pPlayer_(nullptr) {};
	const char* GetName() const override { return "Avo"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
	void Initialize() override;
};

