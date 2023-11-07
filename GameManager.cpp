#include "GameManager.h"
#include "Engine/Input.h"
#include "EnemySpawnCtrl.h"
#include "NavigationAI.h"
#include "Player.h"
#include "Stage.h"
#include "CollisionMap.h"
#include "DamageCtrl.h"
#include "WeaponObjectManager.h"
#include "DropTable.h"

GameManager::GameManager(GameObject* parent)
	: GameObject(parent, "GameManager"), pPlayer_(nullptr), pStage_(nullptr), pEnemySpawnCtrl_(nullptr), pNavigationAI_(nullptr), pDamageCtrl_(nullptr),
	pWeaponObjectManager_(nullptr), pDropTable_(nullptr)
{
}

GameManager::~GameManager()
{
	Release();

}

void GameManager::Initialize()
{
	pEnemySpawnCtrl_ = new EnemySpawnCtrl;
	pEnemySpawnCtrl_->Initialize(this);
	
	pDamageCtrl_ = new DamageCtrl(pEnemySpawnCtrl_);

	pStage_ = Instantiate<Stage>(this);

	pPlayer_ = Instantiate<Player>(this);
	pPlayer_->SetPosition(pStage_->GetPlayerStartPos());

	Instantiate<CollisionMap>(this);
	
	pDropTable_ = new DropTable(this);

	pNavigationAI_ = new NavigationAI(pStage_);
	pWeaponObjectManager_ = new WeaponObjectManager(this);
	
}

void GameManager::Update()
{
	if(pEnemySpawnCtrl_->GetAllEnemy().empty())
		pEnemySpawnCtrl_->SpawnEnemy(ENEMY_FEET);

	//�f�o�b�O�p
	if (Input::IsKeyDown(DIK_M)) pEnemySpawnCtrl_->SpawnEnemy(ENEMY_MASTERHAND);
	if (Input::IsKeyDown(DIK_N)) pEnemySpawnCtrl_->SpawnEnemy(ENEMY_FEET);
	if (Input::IsKeyDown(DIK_K)) pWeaponObjectManager_->AllKillWeaponObject();
}

void GameManager::Draw()
{
}

void GameManager::Release()
{
	pEnemySpawnCtrl_->Release();

}
