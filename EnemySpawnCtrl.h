#pragma once
#include <vector>

class EnemyBase;
class GameObject;

enum ENEMY_TYPE {
	ENEMY_MASTERHAND = 0,
	ENEMY_FEET,
	ENEMY_ASTAR,

	ENEMY_MAX,
};

class EnemySpawnCtrl
{
	std::vector<EnemyBase*> enemyList_;
	GameObject* pParent_;

	void AddEnemyList(EnemyBase* e, int type);

public:
	void Initialize(GameObject* parent);
	void Release();
	
	void AllKillEnemy();
	void KillEnemy(EnemyBase* enemy);
	void SpawnEnemy(int type);
	std::vector<EnemyBase*>& GetAllEnemy();
	void CleanUpEnemyList();

};

