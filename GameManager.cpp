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

//�f�o�b�O�p
#include "Engine/Input.h"

namespace GameManager {
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

		Stage* pStage_ = Instantiate<Stage>(parent);
		Player* pPlayer_ = Instantiate<Player>(parent);
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
		if (Input::IsKey(DIK_TAB)) {
			OutputDebugString("entity : ");
			int count = pEnemySpawnCtrl_->GetAllEnemy().size();
			OutputDebugStringA(std::to_string(count).c_str());
			OutputDebugString("\n");
		}

		//�f�o�b�O�p
		if (Input::IsKeyDown(DIK_M)) { pEnemySpawnCtrl_->SpawnEnemy(ENEMY_MASTERHAND); }
		if (Input::IsKeyDown(DIK_J)) { for (int i = 0; i < 50; i++) pEnemySpawnCtrl_->SpawnEnemy(ENEMY_MASTERHAND); }

		if (Input::IsKeyDown(DIK_N)) { pEnemySpawnCtrl_->SpawnEnemy(ENEMY_FEET); }
		if (Input::IsKeyDown(DIK_H)) { for (int i = 0; i < 50; i++) pEnemySpawnCtrl_->SpawnEnemy(ENEMY_FEET); }
		
		if (Input::IsKeyDown(DIK_K)) { pEnemySpawnCtrl_->SpawnEnemy(ENEMY_ASTAR); }
		if (Input::IsKeyDown(DIK_L)) { for(int i = 0;i < 50;i++) pEnemySpawnCtrl_->SpawnEnemy(ENEMY_ASTAR); }

		if (Input::IsKeyDown(DIK_B)) pWeaponObjectManager_->AllKillWeaponObject();
		if (Input::IsKeyDown(DIK_V)) { pEnemySpawnCtrl_->AllKillEnemy(); }
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

