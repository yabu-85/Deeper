#include "EnemySpawnCtrl.h"
#include "MasterHand.h"
#include "Feet.h"

void EnemySpawnCtrl::Initialize(GameObject* parent)
{
	pParent_ = parent;
}

void EnemySpawnCtrl::Release()
{
	enemyList_.clear();

}

int EnemySpawnCtrl::SpawnEnemy(int type)
{
	CleanUpEnemyList();

	if (type == ENEMY_MASTERHAND) {
		enemyList_.push_back(Instantiate<MasterHand>(pParent_));
		return (int)enemyList_.size() - 1;
	}
	else if (type == ENEMY_FEET) {
		enemyList_.push_back(Instantiate<Feet>(pParent_));
		return (int)enemyList_.size() - 1;
	}

	return 0;
}

std::vector<EnemyBase*>& EnemySpawnCtrl::GetAllEnemy()
{
	CleanUpEnemyList();
	return enemyList_;
}

void EnemySpawnCtrl::CleanUpEnemyList()
{
	for (int i = 0; i < enemyList_.size();) {
		if (enemyList_.at(i)->IsDead())
			enemyList_.erase(enemyList_.begin() + i);
		else
			i++;
	}

	return;

}
