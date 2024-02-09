#pragma once
#include "WeaponBase.h"

class BulletBase : public GameObject
{
protected:
	int hModel_;
	int lifeTime_;		//¶‘¶ŠÔ
	float velocity_;	//’e‘¬
	XMFLOAT3 moveVec_;	//ˆÚ“®—Ê

public:
	BulletBase(GameObject* parent, std::string name);
	virtual ~BulletBase() = 0 {};
	virtual void Initialize() override = 0 {};
	virtual void Update() override = 0 {};
	virtual void Draw() override = 0 {};
	virtual void Release() override = 0 {};

	virtual void Shot(XMFLOAT3 pos, XMFLOAT3 target);	//”­Ë
	virtual void Move();								//ˆÚ“®‚³‚¹‚é
	virtual void LifeTime();							//¶‘¶ŠÔ‚ÌŒvZ
	virtual void LifeDead();							//¶‘¶ŠÔ‚ªI‚í‚Á‚½‚çŒÄ‚ÔŠÖ”

	void SetLifeTime(int i) { lifeTime_ = i; }
	void SetVelocity(float f) { velocity_ = f; }

};

