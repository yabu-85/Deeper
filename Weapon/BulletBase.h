#pragma once
#include "WeaponBase.h"

class BulletBase : public GameObject
{
protected:
	int hModel_;
	int lifeTime_;		//生存時間
	float velocity_;	//弾速
	XMFLOAT3 moveVec_;	//移動量

public:
	BulletBase(GameObject* parent, std::string name);
	virtual ~BulletBase() = 0 {};
	virtual void Initialize() override = 0 {};
	virtual void Update() override = 0 {};
	virtual void Draw() override = 0 {};
	virtual void Release() override = 0 {};

	virtual void Shot(XMFLOAT3 pos, XMFLOAT3 target);	//発射
	virtual void Move();								//移動させる
	virtual void LifeTime();							//生存時間の計算
	virtual void LifeDead();							//生存時間が終わったら呼ぶ関数

	void SetLifeTime(int i) { lifeTime_ = i; }
	void SetVelocity(float f) { velocity_ = f; }

};

