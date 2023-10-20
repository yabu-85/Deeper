#pragma once

class StateManager;

class StateBase
{
protected:
	StateManager* owner_;

public:
	StateBase(StateManager* owner) : owner_ (owner) {}
	virtual const char* GetName() const = 0;
	virtual void Update() = 0;
	virtual void OnEnter() {};
	virtual void OnExit() {};

};

