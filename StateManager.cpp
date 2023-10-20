#include "StateManager.h"
#include "StateBase.h"

StateManager::StateManager(GameObject* owner)
	: owner_(owner), currentState_(nullptr)
{
}

StateManager::~StateManager()
{
    for (auto& mp : stateMap_)
    {
        delete mp.second;
    }
}

void StateManager::Update()
{
    if (currentState_) {
        currentState_->Update();
    }


    return;

    //unordered_mapのすべてのデータにアクセス
    for (auto& pair : stateMap_) {
        StateBase* state = pair.second;
        if (state) {
            state->Update();
        }
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
