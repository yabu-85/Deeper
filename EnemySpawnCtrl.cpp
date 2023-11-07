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

int EnemySpawnCtrl::SpawnEnemy(int type)
{
	CleanUpEnemyList();

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
