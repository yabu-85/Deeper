#pragma once
#include "Engine/Transform.h"

class EnemyBase;

class EnemyUi
{
	int hPict_[2];
	Transform transform_;

	EnemyBase* pParent_;

public:
	EnemyUi(EnemyBase* parent);
	void Initialize();
	void Draw();

};

