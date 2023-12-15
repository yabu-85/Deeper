#include "GameManager.h"
#include "EnemyManager.h"
#include "NavigationAI.h"
#include "Player.h"
#include "Stage.h"
#include "CollisionMap.h"
#include "DamageManager.h"
#include "WeaponObjectManager.h"
#include "DropTable.h"
#include "Engine/GameObject.h"
#include "Engine/Global.h"

#include "Engine/Input.h"

namespace GameManager {
	Player* pPlayer_ = nullptr;
	Stage* pStage_ = nullptr;

	EnemyManager* pEnemyManager_ = nullptr;
	NavigationAI* pNavigationAI_ = nullptr;
	DamageManager* pDamageManager_ = nullptr;
	WeaponObjectManager* pWeaponObjectManager_ = nullptr;
	DropTable* pDropTable_ = nullptr;
	GameObject* pParent_ = nullptr;

	int entityCount = 0;

	void GameManager::Initialize(GameObject* parent)
	{
		pEnemyManager_ = new EnemyManager();
		pEnemyManager_->Initialize(parent);
		pDamageManager_ = new DamageManager(pEnemyManager_);

		pStage_ = Instantiate<Stage>(parent);
		pPlayer_ = Instantiate<Player>(parent);
		pPlayer_->SetPosition(pStage_->GetPlayerStartPos());
		pDamageManager_->AddCharacter(pPlayer_, DamageManager::DA_Player);

		pParent_ = parent;
		Instantiate<CollisionMap>(parent);

		pWeaponObjectManager_ = new WeaponObjectManager();
	
		pDropTable_ = new DropTable();
		pNavigationAI_ = new NavigationAI(pStage_);

	}

	void GameManager::Update()
	{
		if (Input::IsKey(DIK_TAB)) {
			OutputDebugString("entity : ");
			OutputDebugStringA(std::to_string(entityCount).c_str());
			OutputDebugString("\n");
		}

		//デバッグ用
		if (Input::IsKeyDown(DIK_M)) { pEnemyManager_->SpawnEnemy(ENEMY_MASTERHAND); entityCount++; }
		if (Input::IsKeyDown(DIK_J)) { for (int i = 0; i < 20; i++) pEnemyManager_->SpawnEnemy(ENEMY_MASTERHAND);  entityCount += 20; }

		if (Input::IsKeyDown(DIK_N)) { pEnemyManager_->SpawnEnemy(ENEMY_FEET); entityCount++; }
		if (Input::IsKeyDown(DIK_H)) { for (int i = 0; i < 20; i++) pEnemyManager_->SpawnEnemy(ENEMY_FEET);  entityCount += 20; }
		
		if (Input::IsKeyDown(DIK_K)) { pEnemyManager_->SpawnEnemy(ENEMY_ASTAR); entityCount++; }
		if (Input::IsKeyDown(DIK_L)) { for(int i = 0;i < 20;i++) pEnemyManager_->SpawnEnemy(ENEMY_ASTAR);  entityCount+= 20; }

		if (Input::IsKeyDown(DIK_B)) pWeaponObjectManager_->AllKillWeaponObject();
		if (Input::IsKeyDown(DIK_V)) { pEnemyManager_->AllKillEnemy(); entityCount = 0; }
	}

	void GameManager::Release() {
		pEnemyManager_->Release(); 
		SAFE_DELETE(pEnemyManager_);
		SAFE_DELETE(pDamageManager_);
		SAFE_DELETE(pDropTable_);
		SAFE_DELETE(pNavigationAI_);
		SAFE_DELETE(pWeaponObjectManager_);
	}

	EnemyManager* GetEnemyManager() { return pEnemyManager_; }
	NavigationAI* GetNavigationAI() { return pNavigationAI_; }
	DamageManager* GetDamageManager() { return pDamageManager_; }
	WeaponObjectManager* GetWeaponObjectManager() { return pWeaponObjectManager_; }
	DropTable* GetDropTable() { return pDropTable_; }
	GameObject* GetParent() { return pParent_; }
}

