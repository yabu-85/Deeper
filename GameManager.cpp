#include "GameManager.h"
#include "EnemyManager.h"
#include "NavigationAI.h"
#include "Player.h"
#include "CreateStage.h"
#include "CollisionMap.h"
#include "WeaponObjectManager.h"
#include "DropTable.h"
#include "Engine/GameObject.h"
#include "Engine/Global.h"
#include "LifeManager.h"

//デバッグ用
#include "Engine/Input.h"

namespace GameManager {
	EnemyManager* pEnemyManager_ = nullptr;
	NavigationAI* pNavigationAI_ = nullptr;
	WeaponObjectManager* pWeaponObjectManager_ = nullptr;
	DropTable* pDropTable_ = nullptr;
	CollisionMap* pCollisionMap_ = nullptr;
	GameObject* pParent_ = nullptr;
	Player* pPlayer_ = nullptr;
	GameObject* pNowStage_ = nullptr;		//現在の使用されているステージのポインタ格納用
	CreateStage* pCreateStage_ = nullptr;

	void GameManager::Initialize()
	{
		pEnemyManager_ = new EnemyManager();
		pEnemyManager_->Initialize();
		pWeaponObjectManager_ = new WeaponObjectManager();
		pDropTable_ = new DropTable();
		pNavigationAI_ = new NavigationAI();
		pCreateStage_ = new CreateStage();
		pCreateStage_->Initialize();
		LifeManager::Initialize();
	}

	void GameManager::Update()
	{
		pCreateStage_->Update();

		if (Input::IsKey(DIK_TAB)) {
			OutputDebugString("entity : ");
			int count = (int)pEnemyManager_->GetAllEnemy().size();
			OutputDebugStringA(std::to_string(count).c_str());
			OutputDebugString("\n");
		}

		//デバッグ用
		if (Input::IsKeyDown(DIK_M)) { pEnemyManager_->SpawnEnemy(ENEMY_MASTERHAND); }
		if (Input::IsKeyDown(DIK_J)) { for (int i = 0; i < 50; i++) pEnemyManager_->SpawnEnemy(ENEMY_MASTERHAND); }

		if (Input::IsKeyDown(DIK_N)) { pEnemyManager_->SpawnEnemy(ENEMY_FEET); }
		if (Input::IsKeyDown(DIK_H)) { for (int i = 0; i < 50; i++) pEnemyManager_->SpawnEnemy(ENEMY_FEET); }
		
		if (Input::IsKeyDown(DIK_K)) { pEnemyManager_->SpawnEnemy(ENEMY_ASTAR); }
		if (Input::IsKeyDown(DIK_L)) { for(int i = 0;i < 50;i++) pEnemyManager_->SpawnEnemy(ENEMY_ASTAR); }

		if (Input::IsKeyDown(DIK_B)) pWeaponObjectManager_->AllKillWeaponObject();
		if (Input::IsKeyDown(DIK_V)) { pEnemyManager_->AllKillEnemy(); }
	}

	void GameManager::Release() {
		pEnemyManager_->Release(); 
		SAFE_DELETE(pEnemyManager_);
		SAFE_DELETE(pDropTable_);
		SAFE_DELETE(pNavigationAI_);
		SAFE_DELETE(pWeaponObjectManager_);
	}

	void Draw()
	{
		LifeManager::Draw();

	}

	EnemyManager* GetEnemyManager() { return pEnemyManager_; }
	NavigationAI* GetNavigationAI() { return pNavigationAI_; }
	WeaponObjectManager* GetWeaponObjectManager() { return pWeaponObjectManager_; }
	DropTable* GetDropTable() { return pDropTable_; }
	GameObject* GetParent() { return pParent_; }
	CreateStage* GetCreateStage() { return pCreateStage_; }

	void SetCollisionMap(CollisionMap* map) { pCollisionMap_ = map; }
	CollisionMap* GetCollisionMap() { return pCollisionMap_; }

	void SetPlayer(Player* player) { pPlayer_ = player; }
	Player* GetPlayer() { return pPlayer_; }

	void GameManager::SetStage(GameObject* stage) { pNowStage_ = stage; }
	GameObject* GameManager::GetStage() { return pNowStage_; }

}

