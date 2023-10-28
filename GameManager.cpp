#include "GameManager.h"
#include "Engine/Input.h"
#include "EnemySpawnCtrl.h"
#include "NavigationAI.h"
#include "Player.h"
#include "Stage.h"
#include "CollisionMap.h"

GameManager::GameManager(GameObject* parent)
	: GameObject(parent, "GameManager"), pPlayer_(nullptr), pStage_(nullptr), pEnemySpawnCtrl_(nullptr), pNavigationAI_(nullptr)
{
}

GameManager::~GameManager()
{
	Release();

}

void GameManager::Initialize()
{
	pStage_ = Instantiate<Stage>(this);
	pPlayer_ = Instantiate<Player>(this);
	pPlayer_->SetPosition(pStage_->GetPlayerStartPos());

	Instantiate<CollisionMap>(this);

	pNavigationAI_ = new NavigationAI;

	pEnemySpawnCtrl_ = new EnemySpawnCtrl;
	pEnemySpawnCtrl_->Initialize(this);

}

void GameManager::Update()
{


	//デバッグ用
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
