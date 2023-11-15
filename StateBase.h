#pragma once

class StateManager;

class StateBase
{
protected:
	StateManager* owner_;

public:
	virtual ~StateBase() {};
	virtual const char* GetName() const = 0;
	virtual void Update() = 0;
	virtual void OnEnter() {};
	virtual void OnExit() {};
	virtual void Initialize() {};

};

