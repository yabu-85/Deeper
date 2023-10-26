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
	Aim* pAim_;

public:
	EnemyUi(EnemyBase* parent);
	void Initialize();
	void Update();

};

