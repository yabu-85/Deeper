#pragma once
#include <vector>

class EnemyBase;
class GameObject;

enum ENEMY_TYPE {
	ENEMY_MASTERHAND = 0,
	ENEMY_FEET,

	ENEMY_MAX,
};

class EnemySpawnCtrl
{
	std::vector<EnemyBase*> enemyList_;
	GameObject* pParent_;

public:
	void Initialize(GameObject* parent);
	void Release();
	
	int SpawnEnemy(int type);
	std::vector<EnemyBase*>& GetAllEnemy();
	void CleanUpEnemyList();

};

