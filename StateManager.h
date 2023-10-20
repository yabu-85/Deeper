#pragma once
#include <unordered_map>
#include <string>

class State;
class GameObject;

class StateManager
{
	// ＜ステート名、ステート＞map
	std::unordered_map<std::string, State*> stateMap_;
	State* currentState_;
	GameObject* owner_;

public:
	StateManager(GameObject* owner);
	~StateManager();

	void Update();
	void ChangeState(const std::string& name);

	// 新しいステートをmapに追加
	void RegisterState(State* state);
	GameObject* GetActor() { return owner_; }
	std::string GetName();

};

