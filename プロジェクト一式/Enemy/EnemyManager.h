#pragma once
#include <vector>
#include <DirectXMath.h>
using namespace DirectX;

class EnemyBase;
class GameObject;

enum ENEMY_TYPE {
	ENEMY_STONEGOLEM = 0,
	ENEMY_THROW,
	ENEMY_MELEE,
	ENEMY_SWORDBOSS,
	ENEMY_MAX,
};

enum ENEMY_TABLE {
	ETABLE_NORMAL = 0,
	ETABLE_HARD,
	ETABLE_MAX,
};

class EnemyManager
{
	std::vector<EnemyBase*> enemyList_;
	GameObject* pParent_;

	void AddEnemyList(EnemyBase* e, int type);

public:
	EnemyManager();
	void SetParent(GameObject* parent) { pParent_ = parent; }
	std::vector<EnemyBase*>& GetAllEnemy();

	//敵倒す
	void AllKillEnemy();
	void KillEnemy(EnemyBase* enemy);

	//指定した敵
	void SpawnEnemy(ENEMY_TYPE type);

	//Tableから指定して敵出す
	void SpawnEnemyTable(ENEMY_TABLE type);

	//音の範囲内のPatrolStateのEnemyの状態を変更
	void PlayAtPosition(XMFLOAT3 position, float range);
	
	//全エネミーのColliderの判定をtrueに
	void ResetAllEnemyCollider();

	void SceneTransitionInitialize();

	bool IsEnemyListEmpty(){ return enemyList_.empty(); }

};

