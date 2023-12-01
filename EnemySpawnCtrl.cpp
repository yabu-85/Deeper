#include "EnemySpawnCtrl.h"
#include "GameManager.h"
#include "MasterHand.h"
#include "DamageCtrl.h"
#include "Feet.h"
#include "AStarMan.h"

void EnemySpawnCtrl::Initialize(GameObject* parent)
{
	pParent_ = parent;
}

void EnemySpawnCtrl::Release()
{
	enemyList_.clear();
}

void EnemySpawnCtrl::AllKillEnemy()
{
	for (auto it = enemyList_.begin(); it != enemyList_.end();) {
		(*it)->KillMe();
		it = enemyList_.erase(it);
	}
	enemyList_.clear();
}

void EnemySpawnCtrl::KillEnemy(EnemyBase* enemy)
{
	for (auto it = enemyList_.begin(); it != enemyList_.end();) {
		if (*it == enemy) {
			it = enemyList_.erase(it);
			GameManager::GetDamageCtrl()->RemoveCharacter(*it);
			break;
		}
		else {
			++it;
		}
	}
	enemy->KillMe();
}

void EnemySpawnCtrl::SpawnEnemy(int type)
{
	if (type == ENEMY_MASTERHAND) {
		MasterHand* e = Instantiate<MasterHand>(pParent_);
		AddEnemyList(e, type);
	}
	else if (type == ENEMY_FEET) {
		Feet* e = Instantiate<Feet>(pParent_);
		AddEnemyList(e, type);
		GameManager::GetDamageCtrl()->AddCharacter(e, DamageCtrl::DamageType::DA_Enemy);
	}
	else if (type == ENEMY_ASTAR) {
		AStarMan* e = Instantiate<AStarMan>(pParent_);
		AddEnemyList(e, type);
	}
}

std::vector<EnemyBase*>& EnemySpawnCtrl::GetAllEnemy()
{
	return enemyList_;
}

void EnemySpawnCtrl::AddEnemyList(EnemyBase* e, int type)
{
	e->SetEnemyType(ENEMY_TYPE(type));
	enemyList_.push_back(e);
}
