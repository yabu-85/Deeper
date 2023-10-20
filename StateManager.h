#pragma once
#include <unordered_map>
#include <string>

class StateBase;
class GameObject;

class StateManager
{
	// ���X�e�[�g���A�X�e�[�g��map
	std::unordered_map<std::string, StateBase*> stateMap_;
	StateBase* currentState_;
	GameObject* owner_;

public:
	StateManager(GameObject* owner);
	~StateManager();

	void Update();
	void ChangeState(const std::string& name);

	// �V�����X�e�[�g��map�ɒǉ�
	void AddState(StateBase* state);
	GameObject* GetActor() { return owner_; }
	std::string GetName();

};

