#include "EnemyManager.h"
#include "GameManager.h"
#include "MasterHand.h"
#include "DamageManager.h"
#include "Feet.h"
#include "AStarMan.h"

EnemyManager::EnemyManager() : pParent_(nullptr)
{
}

void EnemyManager::Initialize(GameObject* parent)
{
	pParent_ = parent;
}

void EnemyManager::Release()
{
	enemyList_.clear();
}

void EnemyManager::AllKillEnemy()
{
	for (auto it = enemyList_.begin(); it != enemyList_.end();) {
		(*it)->KillMe();
		Character* obj = static_cast<Character*>(*it);
		GameManager::GetDamageManager()->RemoveCharacter(obj);
		it = enemyList_.erase(it);
	}
	enemyList_.clear();
}

void EnemyManager::KillEnemy(EnemyBase* enemy)
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
	Character* obj = static_cast<Character*>(enemy);
	GameManager::GetDamageManager()->RemoveCharacter(obj);
	enemy->KillMe();
}

void EnemyManager::SpawnEnemy(int type)
{
	if (type == ENEMY_MASTERHAND) {
		MasterHand* e = Instantiate<MasterHand>(pParent_);
		AddEnemyList(e, type);
	}
	else if (type == ENEMY_FEET) {
		Feet* e = Instantiate<Feet>(pParent_);
		AddEnemyList(e, type);
	}
	else if (type == ENEMY_ASTAR) {
		AStarMan* e = Instantiate<AStarMan>(pParent_);
		AddEnemyList(e, type);
	}
}

std::vector<EnemyBase*>& EnemyManager::GetAllEnemy()
{
	return enemyList_;
}

void EnemyManager::CleanUpEnemyList()
{
}

void EnemyManager::AddEnemyList(EnemyBase* e, int type)
{
	e->SetEnemyType(ENEMY_TYPE(type));
	enemyList_.push_back(e);
}
