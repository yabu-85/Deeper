#pragma once
#include "Engine/Transform.h"
#include "Engine/VFX.h"

class EnemyBase;
class Aim;

class EnemyUi
{
	int vHandle_[2];
	EmitterData data_;
	Transform transform_;
	EnemyBase* pParent_;

	float parcent;
	XMFLOAT2 size;

public:
	EnemyUi(EnemyBase* parent);
	~EnemyUi();
	void Initialize();
	void Update();

	void SetParcent(float f) { parcent = f; };

};

