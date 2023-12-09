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
#include "Engine/Global.h"

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

	int entityCount = 0;

	void GameManager::Initialize(GameObject* parent)
	{
		pEnemySpawnCtrl_ = new EnemySpawnCtrl;
		pEnemySpawnCtrl_->Initialize(parent);
		pDamageCtrl_ = new DamageCtrl(pEnemySpawnCtrl_);

		pStage_ = Instantiate<Stage>(parent);
		pPlayer_ = Instantiate<Player>(parent);
		pPlayer_->SetPosition(pStage_->GetPlayerStartPos());
		pDamageCtrl_->AddCharacter(pPlayer_, DamageCtrl::DA_Player);

		pParent_ = parent;
		Instantiate<CollisionMap>(parent);

		pWeaponObjectManager_ = new WeaponObjectManager();
	
		pDropTable_ = new DropTable();
		pNavigationAI_ = new NavigationAI(pStage_);

	}

	void GameManager::Update()
	{
		if (Input::IsKeyDown(DIK_TAB)) {
			OutputDebugString("entity : ");
			OutputDebugStringA(std::to_string(entityCount).c_str());
			OutputDebugString("\n");
		}

		//デバッグ用
		if (Input::IsKeyDown(DIK_M)) { pEnemySpawnCtrl_->SpawnEnemy(ENEMY_MASTERHAND); entityCount++; }
		if (Input::IsKeyDown(DIK_J)) { for (int i = 0; i < 20; i++) pEnemySpawnCtrl_->SpawnEnemy(ENEMY_MASTERHAND);  entityCount += 20; }

		if (Input::IsKeyDown(DIK_N)) { pEnemySpawnCtrl_->SpawnEnemy(ENEMY_FEET); entityCount++; }
		if (Input::IsKeyDown(DIK_H)) { for (int i = 0; i < 20; i++) pEnemySpawnCtrl_->SpawnEnemy(ENEMY_FEET);  entityCount += 20; }
		
		if (Input::IsKeyDown(DIK_K)) { pEnemySpawnCtrl_->SpawnEnemy(ENEMY_ASTAR); entityCount++; }
		if (Input::IsKeyDown(DIK_L)) { for(int i = 0;i < 20;i++) pEnemySpawnCtrl_->SpawnEnemy(ENEMY_ASTAR);  entityCount+= 20; }

		if (Input::IsKeyDown(DIK_B)) pWeaponObjectManager_->AllKillWeaponObject();
		if (Input::IsKeyDown(DIK_V)) { pEnemySpawnCtrl_->AllKillEnemy(); entityCount = 0; }
	}

	void GameManager::Release() {
		pEnemySpawnCtrl_->Release(); 
		SAFE_DELETE(pEnemySpawnCtrl_);
		SAFE_DELETE(pDamageCtrl_);
		SAFE_DELETE(pDropTable_);
		SAFE_DELETE(pNavigationAI_);
		SAFE_DELETE(pWeaponObjectManager_);
	}

	EnemySpawnCtrl* GetEnemySpawnCtrl() { return pEnemySpawnCtrl_; }
	NavigationAI* GetNavigationAI() { return pNavigationAI_; }
	DamageCtrl* GetDamageCtrl() { return pDamageCtrl_; }
	WeaponObjectManager* GetWeaponObjectManager() { return pWeaponObjectManager_; }
	DropTable* GetDropTable() { return pDropTable_; }
	GameObject* GetParent() { return pParent_; }
}

