#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class EnemySpawnCtrl;
class LineCollider;
class SphereCollider;

class DamageCtrl
{
	EnemySpawnCtrl* pEnemySpawnCtrl_;

public:
	DamageCtrl(EnemySpawnCtrl* p);
	~DamageCtrl();

	bool CalcSword(LineCollider* line, int damage);
	bool CalcBullet(SphereCollider* sphere, int damage);

};

