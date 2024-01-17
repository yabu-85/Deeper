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
	void SpawnEnemy(ENEMY_TYPE type);
	std::vector<EnemyBase*>& GetAllEnemy();
	
	//‰¹‚Ì”ÍˆÍ“à‚ÌPatrolState‚ÌEnemy‚Ìó‘Ô‚ğ•ÏX
	void PlayAtPosition(XMFLOAT3 position, float range);
	
	void SceneTransitionInitialize();

	bool IsEnemyListEmpty(){ return enemyList_.empty(); }

};

