#pragma once
#include <DirectXMath.h>
#include "EnemySpawnCtrl.h"

using namespace DirectX;
class GameManager;

class DropTable
{
	struct Table{
		unsigned money_;
		unsigned weaponParcent_;	//0Å`100
		unsigned healingItemParcent_;	//0Å`100
	} table_[ENEMY_TYPE::ENEMY_MAX];

	GameManager* pGameManager_;

public:
	DropTable(GameManager* parent);
	void DropItem(int type, XMFLOAT3 pos);
};

