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
		SWORD = 0,	//���Ƃ��Ŏg��
		AOE,		//�~�^�͈̔�
		CUBE,		//������
		ALL,		//���ׂĂ̓G
		RAND,		//�����_����

	};
	void ApplyDamage(DAMAGE_TYPE type, int d);

	int CalcSword(LineCollider* line);

};

