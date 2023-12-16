#pragma once
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;

class EnemyManager;
class Character;
class Collider;

class DamageManager
{
public:
	enum DamageType {
		DA_Player = 0,
		DA_Enemy,
	};

private:
	EnemyManager* pEnemyManager_;

	struct CollisionData {
		DamageType type;
		Character* objct;
	};
	std::vector<CollisionData> collisionList_;

public:
	DamageManager(EnemyManager* p);
	~DamageManager();

	void AddCharacter(Character* obj, DamageType _type);
	void RemoveCharacter(Character* obj);
	void AllRemoveCharacter();

	bool CalcEnemy(Collider* collider, int damage);
	bool CalcPlyaer(Collider* collider, int damage);

};