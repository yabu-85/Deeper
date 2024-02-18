#pragma once
#include <vector>
#include <DirectXMath.h>
using namespace DirectX;

class EnemyBase;
class GameObject;

enum ENEMY_TYPE {
	ENEMY_STONEGOLEM,
	ENEMY_THROW,
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
	
	//音の範囲内のPatrolStateのEnemyの状態を変更
	void PlayAtPosition(XMFLOAT3 position, float range);
	
	//全エネミーのColliderの判定をtrueに
	void ResetAllEnemyCollider();

	void SceneTransitionInitialize();

	bool IsEnemyListEmpty(){ return enemyList_.empty(); }

};

