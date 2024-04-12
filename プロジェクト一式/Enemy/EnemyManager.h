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

	//�G�|��
	void AllKillEnemy();
	void KillEnemy(EnemyBase* enemy);

	//�w�肵���G
	void SpawnEnemy(ENEMY_TYPE type);

	//Table����w�肵�ēG�o��
	void SpawnEnemyTable(ENEMY_TABLE type);

	//���͈͓̔���PatrolState��Enemy�̏�Ԃ�ύX
	void PlayAtPosition(XMFLOAT3 position, float range);
	
	//�S�G�l�~�[��Collider�̔����true��
	void ResetAllEnemyCollider();

	void SceneTransitionInitialize();

	bool IsEnemyListEmpty(){ return enemyList_.empty(); }

};

