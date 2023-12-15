#pragma once
#include <DirectXMath.h>
#include "EnemyManager.h"

using namespace DirectX;

class DropTable
{
	struct Table{
		unsigned money_;
		unsigned weaponParcent_;	//0Å`100
		unsigned healingItemParcent_;	//0Å`100
	} table_[ENEMY_TYPE::ENEMY_MAX];

public:
	DropTable();
	void DropItem(int type, XMFLOAT3 pos);
};

