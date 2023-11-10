#include "EnemySpawnCtrl.h"
#include "MasterHand.h"
#include "Feet.h"
#include "GameManager.h"

void EnemySpawnCtrl::Initialize(GameManager* parent)
{
	pParent_ = parent;
}

void EnemySpawnCtrl::Release()
{
	enemyList_.clear();
}

void EnemySpawnCtrl::KillEnemy(EnemyBase* enemy)
{
	for (auto it = enemyList_.begin(); it != enemyList_.end();) {
		if (*it == enemy) {
			it = enemyList_.erase(it);
			break;
		}
		else {
			++it;
		}
	}
	enemy->KillMe();
}

int EnemySpawnCtrl::SpawnEnemy(int type)
{
	if (type == ENEMY_MASTERHAND) {
		MasterHand* e = Instantiate<MasterHand>(pParent_);
		e->SetEnemyType(ENEMY_MASTERHAND);
		enemyList_.push_back(e);
		return (int)enemyList_.size() - 1;
	}
	else if (type == ENEMY_FEET) {
		Feet* e = Instantiate<Feet>(pParent_);
		e->SetEnemyType(ENEMY_FEET);
		enemyList_.push_back(e);
		return (int)enemyList_.size() - 1;
	}

	return 0;
}

std::vector<EnemyBase*>& EnemySpawnCtrl::GetAllEnemy()
{
	std::string strNumber = std::to_string(enemyList_.size());
	OutputDebugStringA(strNumber.c_str());
	OutputDebugString("\n");

	return enemyList_;
}