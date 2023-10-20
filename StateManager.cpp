#include "StateManager.h"
#include "StateBase.h"

StateManager::StateManager(GameObject* owner)
	: owner_(owner), currentState_(nullptr)
{
}

StateManager::~StateManager()
{
}

void StateManager::Update()
{
}

void StateManager::ChangeState(const std::string& name)
{
}

void StateManager::AddState(StateBase* state)
{
	stateMap_.emplace(state->GetName(), state);
}

std::string StateManager::GetName()
{
	return std::string();
}
