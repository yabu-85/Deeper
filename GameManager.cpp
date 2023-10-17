#include "GameManager.h"
#include "EnemyBase.h"

GameManager::GameManager(GameObject* parent)
	: GameObject(parent, "GameManager")
{
}

GameManager::~GameManager()
{
}

void GameManager::Initialize()
{
	enemyList_.push_back(Instantiate<EnemyBase>(this));
	
}

void GameManager::Update()
{
	int it = 0;
	for (EnemyBase* e : enemyList_) {
		if (e->IsDead()) {
			enemyList_.erase(enemyList_.begin()+it);
		}else
			it++;
	
	}

}

void GameManager::Draw()
{
}

void GameManager::Release()
{
}
