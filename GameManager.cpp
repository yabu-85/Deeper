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
	enemyList_.push_back(Instantiate<EnemyBase>(this));
	enemyList_.push_back(Instantiate<EnemyBase>(this));
	enemyList_.push_back(Instantiate<EnemyBase>(this));
	
	enemyList_.at(1)->SetPosition(XMFLOAT3(25.0f, 0.0f, 0.0f));
	enemyList_.at(2)->SetPosition(XMFLOAT3(30.0f, 0.0f, 0.0f));
	enemyList_.at(3)->SetPosition(XMFLOAT3(35.0f, 0.0f, 0.0f));

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
