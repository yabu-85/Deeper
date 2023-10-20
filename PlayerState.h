#pragma once
#include "StateBase.h"

//—Dæ“x‚PF‰ñ”ğ
//UŒ‚
//ˆÚ“®

class PlayerWait : public StateBase
{
public:
	PlayerWait(GameObject* owner) : StateBase(owner) {};
	const char* GetName() const override { return "Wait"; }
	void OnEnter() override;
	void Update() override;
};

class PlayerWalk : public StateBase
{
public:
	PlayerWalk(GameObject* owner) : StateBase(owner) {};
	const char* GetName() const override { return "Walk"; }
	void OnEnter() override;
	void Update() override;
};

class PlayerAvo : public StateBase
{
public:
	PlayerAvo(GameObject* owner) : StateBase(owner) {};
	const char* GetName() const override { return "Avo"; }
	void Update() override;
	void OnEnter() override;
	void OnExit() override;
};

