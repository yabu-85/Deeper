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

	//�����g��Ȃ��\��̂��
	bool CalcSword(LineCollider* line, int damage);
	bool CalcBullet(SphereCollider* sphere, int damage);
	bool CalcPlyaer(SphereCollider* sphere, int damage);

};