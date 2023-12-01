#pragma once
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;

class EnemySpawnCtrl;
class GameObject;

//不要
//しょうらいnamespaceにしようね
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

	//もう使わない予定のやつら
	bool CalcSword(LineCollider* line, int damage);
	bool CalcBullet(SphereCollider* sphere, int damage);
	bool CalcPlyaer(SphereCollider* sphere, int damage);

};