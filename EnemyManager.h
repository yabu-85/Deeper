#pragma once
#include <vector>
#include <DirectXMath.h>
using namespace DirectX;

class EnemyBase;
class GameObject;

enum ENEMY_TYPE {
	ENEMY_MASTERHAND = 0,
	ENEMY_FEET,
	ENEMY_ASTAR,
	ENEMY_MAX,
};

class EnemyManager
{
	std::vector<EnemyBase*> enemyList_;
	GameObject* pParent_;

	void AddEnemyList(EnemyBase* e, int type);

public:
	EnemyManager();
	void SetParent(GameObject* parent) { pParent_ = parent; }
	void AllKillEnemy();
	void KillEnemy(EnemyBase* enemy);
	void SpawnEnemy(int type);
	std::vector<EnemyBase*>& GetAllEnemy();
	
	//音の範囲内のPatrolStateのEnemyの状態を変更
	void PlayAtPosition(XMFLOAT3 position, float range);
	
	void SceneTransitionInitialize();

};

