#include "GameManager.h"
#include "VFXManager.h"
#include "WaveManager.h"
#include "DifficultyManager.h"
#include "Enemy/EnemyManager.h"
#include "Stage/NavigationAI.h"
#include "Stage/CreateStage.h"
#include "Stage/CollisionMap.h"
#include "Weapon/WeaponObjectManager.h"
#include "Engine/TransitionEffect.h"
#include "Engine/GameObject.h"
#include "Engine/SceneManager.h"
#include "Player/LifeManager.h"
#include "Player/Player.h"
#include "Player/PlayerData.h"
#include "UI/Interaction.h"

//�f�o�b�O�p
#include "Engine/Input.h"
#include "Enemy/EnemyBase.h"

namespace GameManager {
	EnemyManager* pEnemyManager_ = nullptr;
	NavigationAI* pNavigationAI_ = nullptr;
	WeaponObjectManager* pWeaponObjectManager_ = nullptr;
	CollisionMap* pCollisionMap_ = nullptr;
	Player* pPlayer_ = nullptr;
	GameObject* pNowStage_ = nullptr;
	CreateStage* pCreateStage_ = nullptr;
	SceneManager* pSceneManager_ = nullptr;

	int time_ = 0;
	bool isEnd_ = false;
	std::vector<Character*> characterList_;

	void GameManager::Initialize()
	{
		pEnemyManager_ = new EnemyManager();
		pWeaponObjectManager_ = new WeaponObjectManager();
		pNavigationAI_ = new NavigationAI();
		pCreateStage_ = new CreateStage();
		LifeManager::Initialize();
		PlayerData::Initialize();
		Interaction::Initialize();
		VFXManager::Initialize();
		DifficultyManager::Initialize();
		TransitionEffect::Initialize();
	}

	void GameManager::Update()
	{
		//�����^�C�g���V�[���E���U���g�V�[���ł�Update�͂���Ȃ���Draw����
		if (!GameManager::GetStage()) return;

		WaveManager::Update();
		LifeManager::Update();
		DifficultyManager::Update();
		pCreateStage_->Update();

		//�I��邩�ǂ����̔���
		time_++;
		if (isEnd_) {
			if(time_ >= 230) pSceneManager_->ChangeScene(SCENE_ID_RESULT);

		}
		else if (PlayerData::GetClearStageCount() >= 10 || LifeManager::IsDie()) {
			isEnd_ = true;
			time_ = 0;
		}

		//�f�o�b�O�p
		if (GetStage()) {
			if (Input::IsKeyDown(DIK_M)) { pEnemyManager_->SpawnEnemy(ENEMY_STONEGOLEM); }
			if (Input::IsKeyDown(DIK_N)) { for (int i = 0; i < 20; i++) pEnemyManager_->SpawnEnemy(ENEMY_STONEGOLEM); }
			if (Input::IsKeyDown(DIK_L)) { pEnemyManager_->SpawnEnemy(ENEMY_THROW); }
			if (Input::IsKeyDown(DIK_K)) { for (int i = 0; i < 20; i++) pEnemyManager_->SpawnEnemy(ENEMY_THROW); }

			if (Input::IsKeyDown(DIK_B)) pWeaponObjectManager_->AllKillWeaponObject();
			if (Input::IsKeyDown(DIK_V)) { 
				std::vector<EnemyBase*> eList = pEnemyManager_->GetAllEnemy();
				for (auto e : eList) e->ApplyDamage(1000000); 
			}
		
		}
		if (Input::IsKeyDown(DIK_TAB)) {
			OutputDebugString("entity : ");
			int count = (int)pEnemyManager_->GetAllEnemy().size();
			OutputDebugStringA(std::to_string(count).c_str());
			OutputDebugString("\n");
		}
	}

	void Draw()
	{
		SCENE_ID cs = GetSceneManager()->GetNextSceneID();
		if (cs != SCENE_ID_TITLE && cs != SCENE_ID_RESULT) {
			if (!GameManager::GetStage()) return;

			LifeManager::Draw();
			PlayerData::Draw();
			Interaction::Draw();
		}
		
		TransitionEffect::Draw();

	}

	void SceneTransitionInitialize()
	{
		characterList_.clear();
		pCollisionMap_ = nullptr;
		pPlayer_ = nullptr;
		pNowStage_ = nullptr;
		
		PlayerData::SceneTransitionInitialize();
		Interaction::SceneTransitionInitialize();
		DifficultyManager::SceneTransitionInitialize();
		WaveManager::SceneTransitionInitialize();
		TransitionEffect::SceneTransitionInitialize();

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

	//�ʂɗp�ӂ��������������//�C���ӏ�
	std::vector<Character*> GetCharacterList() { return characterList_; }
	void AddCharacter(Character* c) { characterList_.push_back(static_cast<Character*>(c)); }
	void RemoveCharacter(Character* c) {
		for (auto it = characterList_.begin(); it != characterList_.end();) {
			if (*it == c) {
				it = characterList_.erase(it);
				break;
			}
			else {
				++it;
			}
		}
	}

}

