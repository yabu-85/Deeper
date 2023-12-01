#pragma once
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;

class EnemySpawnCtrl;
class GameObject;

//�s�v
//���傤�炢namespace�ɂ��悤��
class LineCollider;
class SphereCollider;

class DamageCtrl
{
	EnemySpawnCtrl* pEnemySpawnCtrl_;

	enum DamageType {

	};
	struct Character {
		DamageType type;
		GameObject* objct;
	};
	std::vector<Character> calcCharacters_;

public:
	DamageCtrl(EnemySpawnCtrl* p);
	~DamageCtrl();
	void Update();

	//�����g��Ȃ��\��̂��
	bool CalcSword(LineCollider* line, int damage);
	bool CalcBullet(SphereCollider* sphere, int damage);
	bool CalcPlyaer(SphereCollider* sphere, int damage);

};