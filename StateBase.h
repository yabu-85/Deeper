#pragma once

class GameObject;

class StateBase
{
protected:
	GameObject* owner_;

public:
	StateBase(GameObject* owner) : owner_ (owner) {}
	virtual const char* GetName() const = 0;

	virtual void Update() = 0;
	virtual void OnEnter() {};
	virtual void OnExit() {};

};

