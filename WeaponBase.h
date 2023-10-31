#pragma once
#include "Engine/GameObject.h"

class StateManager;

class WeaponBase : public GameObject
{
protected:
	int hModel_;
	bool atkEnd_;
	XMFLOAT3 offsetPosition_;
	XMFLOAT3 offsetRotation_;
	StateManager* pStateManager_;

public:
	WeaponBase(GameObject* parent);
	virtual ~WeaponBase() = 0 {};
	virtual void Initialize() override = 0 {};
	virtual void Update() override = 0 {};
	virtual void Draw() override = 0 {};
	virtual void Release() override = 0 {};
	virtual void ResetState() = 0;

	void CalcOffset();
	void SetOffsetPosition(XMFLOAT3 pos) { offsetPosition_ = pos; }
	void SetOffsetRotate(XMFLOAT3 rot) { offsetRotation_ = rot; }
	void UpdateState();
	void SetAtkEnd(bool b) { atkEnd_ = b; }
	bool IsAtkEnd() { return atkEnd_; }
};

