#pragma once
#include "Engine/Transform.h"
#include "Engine/VFX.h"

class EnemyBase;
class Aim;

class EnemyUi
{
	int vHandle_[2];
	float height_;			//ui�̍���
	float parcent;			//hp�̃p�[�Z���g
	XMFLOAT2 size;			//Ui��Hp�ɍ��킹���T�C�Y
	Transform transform_;
	EnemyBase* pParent_;
	EmitterData data_;



public:
	EnemyUi(EnemyBase* parent);
	~EnemyUi();
	void Initialize(float height); //height�FUi�̍���
	void Update();

	void SetParcent(float f) { parcent = f; };

};

