#include "GameManager.h"
#include "Engine/Input.h"
#include "EnemySpawnCtrl.h"
#include "NavigationAI.h"

GameManager::GameManager(GameObject* parent)
	: GameObject(parent, "GameManager"), pEnemySpawnCtrl_(nullptr), pNavigationAI_(nullptr)
{
}

GameManager::~GameManager()
{
	Release();

}

void GameManager::Initialize()
{
	pNavigationAI_ = new NavigationAI;

	pEnemySpawnCtrl_ = new EnemySpawnCtrl;
	pEnemySpawnCtrl_->Initialize(this);


}

void GameManager::Update()
{
	if (Input::IsKeyDown(DIK_M)) pEnemySpawnCtrl_->SpawnEnemy(ENEMY_MASTERHAND);
	if (Input::IsKeyDown(DIK_N)) pEnemySpawnCtrl_->SpawnEnemy(ENEMY_FEET);

}

void GameManager::Draw()
{
}

void GameManager::Release()
{
	pEnemySpawnCtrl_->Release();

}
