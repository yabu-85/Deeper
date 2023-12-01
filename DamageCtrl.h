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
public:
	enum DamageType {
		DA_Player = 0,
		DA_Enemy,
	};

private:
	EnemySpawnCtrl* pEnemySpawnCtrl_;

	struct Character {
		DamageType type;
		GameObject* objct;
	};
	std::vector<Character> calcCharacters_;

public:
	DamageCtrl(EnemySpawnCtrl* p);
	~DamageCtrl();
	void Update();
	void AddCharacter(GameObject* obj, DamageType _type);
	void RemoveCharacter(GameObject* obj);

	//もう使わない予定のやつら
	bool CalcSword(LineCollider* line, int damage);
	bool CalcBullet(SphereCollider* sphere, int damage);
	bool CalcPlyaer(SphereCollider* sphere, int damage);

};