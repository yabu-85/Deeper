#include "EnemyManager.h"
#include "GameManager.h"
#include "Feet.h"
#include "StateManager.h"

EnemyManager::EnemyManager() : pParent_(nullptr)
{
}

void EnemyManager::AllKillEnemy()
{
	for (auto it = enemyList_.begin(); it != enemyList_.end();) {
		(*it)->KillMe();
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
	enemy->KillMe();
}

void EnemyManager::SpawnEnemy(ENEMY_TYPE type)
{
	if (type == ENEMY_FEET) AddEnemyList(InstantiateFront<Feet>(pParent_), type);

}

std::vector<EnemyBase*>& EnemyManager::GetAllEnemy()
{
	return enemyList_;
}

void EnemyManager::PlayAtPosition(XMFLOAT3 position, float range)
{
	for (auto it : enemyList_) {
		if (it->GetStateManager() && it->GetStateManager()->GetName() == "Patrol") {
			XMFLOAT3 fEnemy = it->GetPosition();
			XMVECTOR ePos = XMLoadFloat3(&fEnemy);
			XMVECTOR sPos = XMLoadFloat3(&position);
			if (XMVectorGetX(XMVector3Length(sPos - ePos)) <= range) {
				it->GetStateManager()->ChangeState("Combat");
			}
		}
	}
}

void EnemyManager::SceneTransitionInitialize()
{
	enemyList_.clear();
	pParent_ = nullptr;

}

void EnemyManager::AddEnemyList(EnemyBase* e, int type)
{
	e->SetEnemyType(ENEMY_TYPE(type));
	enemyList_.push_back(e);
}