#include "GameManager.h"
#include "EnemySpawnCtrl.h"
#include "NavigationAI.h"
#include "Player.h"
#include "Stage.h"
#include "CollisionMap.h"
#include "DamageCtrl.h"
#include "WeaponObjectManager.h"
#include "DropTable.h"
#include "Engine/GameObject.h"

#include "Engine/Input.h"

namespace GameManager {
	Player* pPlayer_ = nullptr;
	Stage* pStage_ = nullptr;
	EnemySpawnCtrl* pEnemySpawnCtrl_ = nullptr;
	NavigationAI* pNavigationAI_ = nullptr;
	DamageCtrl* pDamageCtrl_ = nullptr;
	WeaponObjectManager* pWeaponObjectManager_ = nullptr;
	DropTable* pDropTable_ = nullptr;
	GameObject* pParent_ = nullptr;

	void GameManager::Initialize(GameObject* parent)
	{
		pEnemySpawnCtrl_ = new EnemySpawnCtrl;
		pEnemySpawnCtrl_->Initialize(parent);

		pDamageCtrl_ = new DamageCtrl(pEnemySpawnCtrl_);

		pStage_ = Instantiate<Stage>(parent);

		pPlayer_ = Instantiate<Player>(parent);
		pPlayer_->SetPosition(pStage_->GetPlayerStartPos());

		Instantiate<CollisionMap>(parent);

		pDropTable_ = new DropTable();

		pNavigationAI_ = new NavigationAI(pStage_);
		pWeaponObjectManager_ = new WeaponObjectManager();

		pParent_ = parent;
	}

	void GameManager::Update()
	{
		//デバッグ用
		if (Input::IsKeyDown(DIK_M)) pEnemySpawnCtrl_->SpawnEnemy(ENEMY_MASTERHAND);
		if (Input::IsKeyDown(DIK_N)) pEnemySpawnCtrl_->SpawnEnemy(ENEMY_FEET);
		if (Input::IsKeyDown(DIK_K)) pWeaponObjectManager_->AllKillWeaponObject();
	}

	void GameManager::Release() { 
		pEnemySpawnCtrl_->Release(); 
	}

	EnemySpawnCtrl* GetEnemySpawnCtrl() { return pEnemySpawnCtrl_; }
	NavigationAI* GetNavigationAI() { return pNavigationAI_; }
	DamageCtrl* GetDamageCtrl() { return pDamageCtrl_; }
	WeaponObjectManager* GetWeaponObjectManager() { return pWeaponObjectManager_; }
	DropTable* GetDropTable() { return pDropTable_; }
	GameObject* GetParent() { return pParent_; }

}

