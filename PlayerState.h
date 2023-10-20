#pragma once
#include "StateBase.h"

class PlayerAvo : public StateBase
{

public:
	PlayerAvo(GameObject* owner);
	const char* GetName() const override { return "Avo"; }

	void Update() override;
	void OnEnter() override;
	void OnExit() override;

};

