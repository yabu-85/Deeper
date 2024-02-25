#include "DifficultyManager.h"
#include "GameManager.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/EnemyBase.h"
#include "State/StateManager.h"
#include <vector>

namespace DifficultyManager {
	struct EnemyData
	{
		int enemyPowerLevels;
		EnemyData(int p) : enemyPowerLevels(p) {}
	}data_[ENEMY_MAX]{
		EnemyData(10),	//Stone
		EnemyData(5),	//Throw
	};
	
	//�g�����ǂ����E�E�E	//�C���ӏ�
	int stageTime_;
	int waveTime_;

	int maxDifficulty_ = 50;
	int waveDifficulty_ = 0;

	/*
	��Փx�����߂�p�����[�^
	�@Wave�@
		�N���A����
		HP
	�@�펞
		�Q�[������̓G�̋����̍��v�Ŕ��f
		���U�����Ă���G�̂Ȃ�
		HP
	*/

	//--------------------------------------------

	void Initialize()
	{
	}

	void Update()
	{
		//���݂̓�Փx���v�Z
		waveDifficulty_ = 0;
		std::vector<EnemyBase*> eList = GameManager::GetEnemyManager()->GetAllEnemy();
		if (!eList.empty()) {
			for (auto e : eList) {
				if (e->GetStateManager()->GetName() != "Combat" && e->GetCombatStateManager()->GetName() == "Attack")
					waveDifficulty_ += data_[(int)e->GetEnemyType()].enemyPowerLevels;
			}
		}

	}

	void SceneTransitionInitialize()
	{
		waveDifficulty_ = 0;
	}

	bool AttackPermission()
	{
		if (waveDifficulty_ <= maxDifficulty_) return true;
		return false;
	}

}

