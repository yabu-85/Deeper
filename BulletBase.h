#pragma once
#include "WeaponBase.h"

class BulletBase : public GameObject
{
protected:
	int hModel_;
	int lifeTime_;		//��������
	float velocity_;	//�e��
	XMFLOAT3 moveVec_;	//�ړ���

public:
	BulletBase(GameObject* parent);
	virtual ~BulletBase() = 0 {};
	virtual void Initialize() override = 0 {};
	virtual void Update() override = 0 {};
	virtual void Draw() override = 0 {};
	virtual void Release() override = 0 {};

	virtual void Shot(XMFLOAT3 pos, XMFLOAT3 target);	//����
	virtual void Move();								//�ړ�������
	virtual void LifeTime();

};

