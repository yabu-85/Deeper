#pragma once
#include "../Engine/GameObject.h"

class StateManager;

class WeaponBase : public GameObject
{
protected:
	int hModel_;
	unsigned durance_;				//耐久値
	bool atkEnd_;					//攻撃が終わったかどうかのフラグ
	Transform offsetTrans_;			//移動量
	StateManager* pStateManager_;	

public:
	WeaponBase(GameObject* parent, std::string name);
	virtual ~WeaponBase() = 0 {};
	virtual void Initialize() override = 0 {};
	virtual void Update() override = 0 {};
	virtual void Draw() override = 0 {};
	virtual void Release() override = 0 {};
	virtual void ResetState() = 0;

	void CalcOffset(Transform & trans);
	void SetOffsetPosition(XMFLOAT3 pos) { offsetTrans_.position_ = pos; }
	void SetOffsetRotate(XMFLOAT3 rot) { offsetTrans_.rotate_ = rot; }
	void SetOffsetScale(XMFLOAT3 sca) { offsetTrans_.scale_ = sca; }
	void UpdateState();
	void SetAtkEnd(bool b) { atkEnd_ = b; }
	bool IsAtkEnd() { return atkEnd_; }
	void Endurance(int val = 1) { durance_ -= val; }
	unsigned GetDurability() { return durance_; }
	bool IsBlockend();

};

