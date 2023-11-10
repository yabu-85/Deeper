#pragma once
#include <vector>

class EnemyBase;
class GameManager;

enum ENEMY_TYPE {
	ENEMY_MASTERHAND = 0,
	ENEMY_FEET,

	ENEMY_MAX,
};

class EnemySpawnCtrl
{
	std::vector<EnemyBase*> enemyList_;
	GameManager* pParent_;

public:
	void Initialize(GameManager* parent);
	void Release();
	
	void KillEnemy(EnemyBase* enemy);
	int SpawnEnemy(int type);
	std::vector<EnemyBase*>& GetAllEnemy();
	void CleanUpEnemyList();

};

