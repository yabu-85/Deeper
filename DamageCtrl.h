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
		SWORD = 0,	//Œ•‚Æ‚©‚ÅŽg‚¤
		AOE,		//‰~Œ^‚Ì”ÍˆÍ
		CUBE,		//—§•û‘Ì
		ALL,		//‚·‚×‚Ä‚Ì“G
		RAND,		//ƒ‰ƒ“ƒ_ƒ€‚É

	};
	void ApplyDamage(DAMAGE_TYPE type, int d);

	int CalcSword(LineCollider* line);

};

