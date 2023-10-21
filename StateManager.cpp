#include "StateManager.h"
#include "StateBase.h"

StateManager::StateManager(GameObject* owner)
	: owner_(owner), currentState_(nullptr)
{
}

StateManager::~StateManager()
{
}

void StateManager::Initialize()
{
	for (auto& data : stateMap_) {
		StateBase* state = data.second;

		if (state) {
			state->Initialize();
		}
	}
}

void StateManager::Update()
{
    if (currentState_) {
        currentState_->Update();
    }

}

void StateManager::ChangeState(const std::string& name)
{
	// 現在のステートから出る
	if (currentState_)
	{
		currentState_->OnExit();
	}

	// mapから新しいステートを探す
	auto iter = stateMap_.find(name);
	if (iter != stateMap_.end())
	{
		currentState_ = iter->second;
		// 新しい状態に入る
		currentState_->OnEnter();
	}
	else
	{
		currentState_ = nullptr;
	}

}

void StateManager::AddState(StateBase* state)
{
	stateMap_.emplace(state->GetName(), state);
}

std::string StateManager::GetName()
{
	return currentState_->GetName();
}
