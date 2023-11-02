#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class EnemySpawnCtrl;
class LineCollider;

class DamageCtrl
{
	EnemySpawnCtrl* pEnemySpawnCtrl_;

public:
	DamageCtrl(EnemySpawnCtrl* p);
	~DamageCtrl();

	enum DAMAGE_TYPE {
		SWORD = 0,	//剣とかで使う
		AOE,		//円型の範囲
		CUBE,		//立方体
		ALL,		//すべての敵
		RAND,		//ランダムに

	};
	void ApplyDamage(DAMAGE_TYPE type, int d);

	int CalcSword(LineCollider* line);

};

