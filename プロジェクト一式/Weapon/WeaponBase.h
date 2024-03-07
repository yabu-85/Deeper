#pragma once
#include "../Engine/GameObject.h"

class StateManager;

class WeaponBase : public GameObject
{
protected:
	int hModel_;
	int boneIndex_;
	int partIndex_;
	unsigned durance_;				//�ϋv�l
	bool atkEnd_;					//�U�����I��������ǂ����̃t���O
	StateManager* pStateManager_;

public:
	WeaponBase(GameObject* parent, std::string name);
	virtual ~WeaponBase() = 0 {};
	virtual void Initialize() override = 0 {};
	virtual void Update() override = 0 {};
	virtual void Draw() override = 0 {};
	virtual void Release() override = 0 {};
	virtual void ResetState() = 0;

	void UpdateState();
	void SetAtkEnd(bool b) { atkEnd_ = b; }
	bool IsAtkEnd() { return atkEnd_; }
	virtual void ChangeAttackState() {};
	void Endurance(int val = 1) { durance_ -= val; }
	unsigned GetDurability() { return durance_; }
	bool IsBlockend();

};

