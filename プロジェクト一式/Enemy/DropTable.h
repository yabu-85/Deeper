#pragma once
#include <DirectXMath.h>
#include "EnemyManager.h"

using namespace DirectX;

/// <summary>
/// 敵のドロップをまとめた
/// </summary>
namespace DropTable
{
	void DropItem(ENEMY_TYPE type, XMFLOAT3 pos);

};

