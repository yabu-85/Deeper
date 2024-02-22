#include "DifficultyManager.h"
#include "GameManager.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/EnemyBase.h"
#include "State/StateManager.h"
#include <vector>

namespace DifficultyManager {
	static const int MAX_DIFFICULTY = 50;
	struct EnemyData
	{
		int enemyPowerLevels;
		EnemyData(int p) : enemyPowerLevels(p) {}
	}static const data_[ENEMY_MAX]{
		EnemyData(10),	//Stone
		EnemyData(5),	//Throw
	};
	
	int difficulty = 0;

	//--------------------------------------------

	void Initialize()
	{

	}

	void Update()
	{
		//�C���ӏ�
		//������Enemy�̒���MainState��Combat�̂�̒������ԍ����Ώۂ��v�Z
		//����Enemy�͏펞���X�g�ɒǉ��݂����Ȋ�����

		//��Փx�̌v�Z
		difficulty = 0;
		std::vector<EnemyBase*> eList = GameManager::GetEnemyManager()->GetAllEnemy();
		if (!eList.empty()) {
			for (auto e : eList) {
				if (e->GetStateManager()->GetName() != "Combat" && e->GetCombatStateManager()->GetName() == "Attack")
					difficulty += data_[(int)e->GetEnemyType()].enemyPowerLevels;
			}
		}

	}

	void SceneTransitionInitialize()
	{
		difficulty = 0;

	}

	bool AttackPermission()
	{
		if (difficulty <= MAX_DIFFICULTY) return true;
		return false;
	}

}

