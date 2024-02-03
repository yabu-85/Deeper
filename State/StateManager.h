#pragma once
#include <unordered_map>
#include <string>

class StateBase;
class GameObject;

class StateManager
{
	// ＜ステート名、ステート＞map
	std::unordered_map<std::string, StateBase*> stateMap_;
	StateBase* currentState_;
	GameObject* owner_;
	
public:
	StateManager(GameObject* owner);
	~StateManager();

	void Initialize();
	void Update();
	void ChangeState(const std::string& name);

	// 新しいステートをmapに追加
	void AddState(StateBase* state);
	GameObject* GetGameObject() { return owner_; }
	std::string GetName();

};

