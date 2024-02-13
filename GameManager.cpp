#include "GameManager.h"
#include "Enemy/EnemyManager.h"
#include "Stage/NavigationAI.h"
#include "Player/Player.h"
#include "Stage/CreateStage.h"
#include "Stage/CollisionMap.h"
#include "Weapon/WeaponObjectManager.h"
#include "Engine/GameObject.h"
#include "Engine/Global.h"
#include "Player/LifeManager.h"
#include "Player/PlayerData.h"
#include "UI/Interaction.h"
#include "VFXManager.h"
#include "Engine/SceneManager.h"

//デバッグ用
#include "Engine/Input.h"

namespace GameManager {
	EnemyManager* pEnemyManager_ = nullptr;
	NavigationAI* pNavigationAI_ = nullptr;
	WeaponObjectManager* pWeaponObjectManager_ = nullptr;
	CollisionMap* pCollisionMap_ = nullptr;
	Player* pPlayer_ = nullptr;
	GameObject* pNowStage_ = nullptr;
	CreateStage* pCreateStage_ = nullptr;
	SceneManager* pSceneManager_ = nullptr;

	void GameManager::Initialize()
	{
		pEnemyManager_ = new EnemyManager();
		pWeaponObjectManager_ = new WeaponObjectManager();
		pNavigationAI_ = new NavigationAI();
		pCreateStage_ = new CreateStage();
		PlayerData::Initialize();
		Interaction::Initialize();
		VFXManager::Initialize();

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
			if (Input::IsKeyDown(DIK_M)) { pEnemyManager_->SpawnEnemy(ENEMY_FEET); }
			if (Input::IsKeyDown(DIK_N)) { for (int i = 0; i < 20; i++) pEnemyManager_->SpawnEnemy(ENEMY_FEET); }
			if (Input::IsKeyDown(DIK_L)) { pEnemyManager_->SpawnEnemy(ENEMY_THROW); }
			if (Input::IsKeyDown(DIK_K)) { for (int i = 0; i < 20; i++) pEnemyManager_->SpawnEnemy(ENEMY_THROW); }

			if (Input::IsKeyDown(DIK_B)) pWeaponObjectManager_->AllKillWeaponObject();
			if (Input::IsKeyDown(DIK_V)) { pEnemyManager_->AllKillEnemy(); }
		}
	}

	void GameManager::Release() {
		SAFE_DELETE(pEnemyManager_);
		SAFE_DELETE(pNavigationAI_);
		SAFE_DELETE(pWeaponObjectManager_);
	}

	void Draw()
	{
		PlayerData::Draw();
		Interaction::Draw();

	}

	void SceneTransitionInitialize()
	{
		pCollisionMap_ = nullptr;
		pPlayer_ = nullptr;
		pNowStage_ = nullptr;

		PlayerData::SceneTransitionInitialize();
		pEnemyManager_->SceneTransitionInitialize();
		pWeaponObjectManager_->SceneTransitionInitialize();

	}

	EnemyManager* GetEnemyManager() { return pEnemyManager_; }
	NavigationAI* GetNavigationAI() { return pNavigationAI_; }
	WeaponObjectManager* GetWeaponObjectManager() { return pWeaponObjectManager_; }
	CreateStage* GetCreateStage() { return pCreateStage_; }

	SceneManager* GetSceneManager() { return pSceneManager_; }
	void SetSceneManager(SceneManager* ma) { pSceneManager_ = ma; }
	CollisionMap* GetCollisionMap() { return pCollisionMap_; }
	void SetCollisionMap(CollisionMap* map) { pCollisionMap_ = map; }
	Player* GetPlayer() { return pPlayer_; }
	void SetPlayer(Player* player) { pPlayer_ = player; }
	GameObject* GetStage() { return pNowStage_; }
	void SetStage(GameObject* stage) { pNowStage_ = stage; pEnemyManager_->SetParent(stage); }

}

