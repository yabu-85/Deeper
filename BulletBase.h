#pragma once
#include "Engine/GameObject.h"
#include "WeaponBase.h"

class BulletBase : public GameObject
{
protected:
	int hModel_;
	int lifeTime_;		//生存時間
	XMFLOAT3 moveVec_;	//移動量

public:
	BulletBase(GameObject* parent);
	virtual ~BulletBase() = 0 {};
	virtual void Initialize() override = 0 {};
	virtual void Update() override = 0 {};
	virtual void Draw() override = 0 {};
	virtual void Release() override = 0 {};

	virtual void Shot(XMFLOAT3 pos, XMFLOAT3 target);	//発射
	virtual void Move();								//移動させる

};

