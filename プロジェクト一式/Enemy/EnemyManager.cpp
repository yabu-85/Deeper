#include "EnemyManager.h"
#include "../GameManager/GameManager.h"
#include "../State/StateManager.h"
#include "StoneGolem.h"
#include "ThrowEnemy.h"
#include "MeleeFighter.h"

namespace {
	static const std::vector<std::vector<ENEMY_TYPE>> spawnEnemyTable[ETABLE_MAX] = {
		{
			{ ENEMY_MELEE, ENEMY_MELEE, ENEMY_THROW, ENEMY_THROW},
			{ ENEMY_STONEGOLEM, ENEMY_STONEGOLEM, ENEMY_MELEE, ENEMY_MELEE },
			{ ENEMY_STONEGOLEM, ENEMY_MELEE, ENEMY_THROW, ENEMY_THROW},
			{ ENEMY_STONEGOLEM, ENEMY_STONEGOLEM, ENEMY_THROW, ENEMY_THROW},
		}
	};
}

EnemyManager::EnemyManager() : pParent_(nullptr)
{
}

void EnemyManager::AllKillEnemy()
{
	for (auto it = enemyList_.begin(); it != enemyList_.end();) {
		EnemyBase* erasedEnemy = static_cast<EnemyBase*>(*it);
	
		(*it)->KillMe();
		it = enemyList_.erase(it);
	}
	enemyList_.clear();
}

void EnemyManager::KillEnemy(EnemyBase* enemy)
{
	for (auto it = enemyList_.begin(); it != enemyList_.end();) {
		if (*it == enemy) {
			EnemyBase* erasedEnemy = static_cast<EnemyBase*>(*it);
			
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
	if (type == ENEMY_STONEGOLEM) AddEnemyList(InstantiateFront<StoneGolem>(pParent_), type);
	else if (type == ENEMY_THROW) AddEnemyList(InstantiateFront<ThrowEnemy>(pParent_), type);
	else if (type == ENEMY_MELEE) AddEnemyList(InstantiateFront<MeleeFighter>(pParent_), type);
}

void EnemyManager::SpawnEnemyTable(ENEMY_TABLE type)
{
	//spawnEnemyTableÇÃíÜÇ©ÇÁspawnTableÇÃèÍèäÇ©ÇÁÇ∆Ç¡ÇƒÇªÇÃvectorÇÃsizeÇéÊìæ
	int randMax = (int)spawnEnemyTable[type].size();
	if (randMax <= 0) return;

	//Ç±Ç±ìÔà’ìxÇ…ÇÊÇ¡ÇƒëIÇ‘ämó¶êßå‰Ç∆Ç©ÇµÇΩÇ¢
	int r = rand() % randMax;

	int max = (int)spawnEnemyTable[type][r].size();
	for (int i = 0; i < max; i++) {
		SpawnEnemy(spawnEnemyTable[type][r].at(i));
	}
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

void EnemyManager::ResetAllEnemyCollider()
{
	for (auto it : enemyList_) {
		std::list<Collider*> cList = it->GetColliderList();
		for (auto c : cList) c->SetValid(true);
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