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
	Player* pPlayer_ = nullptr;
	GameObject* pNowStage_ = nullptr;
	CreateStage* pCreateStage_ = nullptr;

	void GameManager::Initialize()
	{
		pEnemyManager_ = new EnemyManager();
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

		if (Input::IsKeyDown(DIK_TAB)) {
			OutputDebugString("entity : ");
			int count = (int)pEnemyManager_->GetAllEnemy().size();
			OutputDebugStringA(std::to_string(count).c_str());
			OutputDebugString("\n");
		}

		//デバッグ用
		if (GetStage()) {
			if (Input::IsKeyDown(DIK_M)) { pEnemyManager_->SpawnEnemy(ENEMY_MASTERHAND); }
			if (Input::IsKeyDown(DIK_J)) { for (int i = 0; i < 25; i++) pEnemyManager_->SpawnEnemy(ENEMY_MASTERHAND); }

			if (Input::IsKeyDown(DIK_N)) { pEnemyManager_->SpawnEnemy(ENEMY_FEET); }
			if (Input::IsKeyDown(DIK_H)) { for (int i = 0; i < 25; i++) pEnemyManager_->SpawnEnemy(ENEMY_FEET); }

			if (Input::IsKeyDown(DIK_K)) { pEnemyManager_->SpawnEnemy(ENEMY_ASTAR); }
			if (Input::IsKeyDown(DIK_L)) { for (int i = 0; i < 25; i++) pEnemyManager_->SpawnEnemy(ENEMY_ASTAR); }

			if (Input::IsKeyDown(DIK_B)) pWeaponObjectManager_->AllKillWeaponObject();
			if (Input::IsKeyDown(DIK_V)) { pEnemyManager_->AllKillEnemy(); }
		}
		
	}

	void GameManager::Release() {
		SAFE_DELETE(pEnemyManager_);
		SAFE_DELETE(pDropTable_);
		SAFE_DELETE(pNavigationAI_);
		SAFE_DELETE(pWeaponObjectManager_);
	}

	void Draw()
	{
		pWeaponObjectManager_;
		LifeManager::Draw();

	}

	void SceneTransitionInitialize()
	{
		pCollisionMap_ = nullptr;
		pPlayer_ = nullptr;
		pNowStage_ = nullptr;
	}

	EnemyManager* GetEnemyManager() { return pEnemyManager_; }
	NavigationAI* GetNavigationAI() { return pNavigationAI_; }
	WeaponObjectManager* GetWeaponObjectManager() { return pWeaponObjectManager_; }
	DropTable* GetDropTable() { return pDropTable_; }
	CreateStage* GetCreateStage() { return pCreateStage_; }

	CollisionMap* GetCollisionMap() { return pCollisionMap_; }
	void SetCollisionMap(CollisionMap* map) { pCollisionMap_ = map; }
	Player* GetPlayer() { return pPlayer_; }
	void SetPlayer(Player* player) { pPlayer_ = player; }

	GameObject* GetStage() { return pNowStage_; }
	void SetStage(GameObject* stage) {
		pNowStage_ = stage;
		pEnemyManager_->SetParent(stage); 
	}

}

