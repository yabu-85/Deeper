#pragma once
#include "../Engine/GameObject.h"
#include "WeaponObjectManager.h"

class StateManager;

class WeaponBase : public GameObject
{
protected:
	WeaponObjectManager::WEAPON_TYPE type_;
	int hModel_;
	int boneIndex_;
	int partIndex_;
	unsigned durance_;				//耐久値
	bool atkEnd_;					//攻撃が終わったかどうかのフラグ
	StateManager* pStateManager_;

public:
	WeaponBase(GameObject* parent, std::string name);
	virtual ~WeaponBase() = 0 {};
	virtual void Initialize() override = 0 {};
	virtual void Update() override = 0 {};
	virtual void Draw() override = 0 {};
	virtual void Release() override = 0 {};
	virtual void ResetState() = 0;

	WeaponObjectManager::WEAPON_TYPE GetWeaponType() { return type_; }
	void UpdateState();
	void SetAtkEnd(bool b) { atkEnd_ = b; }
	bool IsAtkEnd() { return atkEnd_; }
	virtual void ChangeAttackState() {};
	void Endurance(int val = 1) { durance_ -= val; }
	unsigned GetDurability() { return durance_; }
	void SetDurability(int i) { durance_ = i; }
	bool IsBlockend();

};

