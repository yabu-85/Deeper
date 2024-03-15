#include "GameManager.h"
#include "WaveManager.h"
#include "DifficultyManager.h"
#include "../VFXManager.h"
#include "../Enemy/EnemyManager.h"
#include "../Stage/NavigationAI.h"
#include "../Stage/CreateStage.h"
#include "../Stage/CollisionMap.h"
#include "../Weapon/WeaponObjectManager.h"
#include "../Engine/TransitionEffect.h"
#include "../Engine/GameObject.h"
#include "../Engine/SceneManager.h"
#include "../Player/LifeManager.h"
#include "../Player/Player.h"
#include "../Player/PlayerData.h"
#include "../UI/Interaction.h"
#include "../Scene/SceneBase.h"

//Pause
#include "../Engine/Model.h"
#include "../Engine/Input.h"
#include "../Character/CharacterManager.h"
#include "../Scene/StageBase.h"
#include "../Player/Aim.h"

//デバッグ用
#include "../Enemy/EnemyBase.h"
#include "../GameManager/CombatAI.h"

namespace GameManager {
	bool isPause_ = false;		//Pause状態かどうか
	bool pauseClose_ = false;	//Keyで閉じる用のフラグ

	EnemyManager* pEnemyManager_ = nullptr;
	NavigationAI* pNavigationAI_ = nullptr;
	WeaponObjectManager* pWeaponObjectManager_ = nullptr;
	CollisionMap* pCollisionMap_ = nullptr;
	Player* pPlayer_ = nullptr;
	StageBase* pStage_ = nullptr;
	CreateStage* pCreateStage_ = nullptr;
	SceneManager* pSceneManager_ = nullptr;

}

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
	//ここタイトルシーン・リザルトシーンでのUpdateはいらないよDrawもね
	if (!GameManager::GetStage()) return;

	//Pause中ならUIをUpdate
	if (isPause_) {
		GetStage()->UIUpdate();
		if (Input::IsKeyDown(DIK_ESCAPE)) {
			pauseClose_ = true;
		}
	}

	MouseLimitedChange();


	DifficultyManager::Update();
	CombatAI::Update();
	WaveManager::Update();
	LifeManager::Update();

	//デバッグ用
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

void GameManager::Draw()
{
	SCENE_ID cs = GetSceneManager()->GetNextSceneID();
	if (cs != SCENE_ID_TITLE && cs != SCENE_ID_RESULT) {
		if (!GameManager::GetStage()) return;

		LifeManager::Draw();
		PlayerData::Draw();
		Interaction::Draw();

		//Pause中ならUIをDraw
		if (isPause_) {
			GetStage()->UIDraw();

		}

	}

	TransitionEffect::Draw();

}

void GameManager::SceneTransitionInitialize()
{
	pCollisionMap_ = nullptr;
	pPlayer_ = nullptr;
	pStage_ = nullptr;

	PlayerData::SceneTransitionInitialize();
	Interaction::SceneTransitionInitialize();
	DifficultyManager::SceneTransitionInitialize();
	WaveManager::SceneTransitionInitialize();
	TransitionEffect::SceneTransitionInitialize();

	pEnemyManager_->SceneTransitionInitialize();
	pWeaponObjectManager_->SceneTransitionInitialize();

	isPause_ = false;

}

void GameManager::StartPause()
{
	isPause_ = true;
	
	//全てのキャラクターのUpdateを拒否
	CharacterManager::SetAllCharacterLeave();

	//全ての弾丸のUpdateを拒否
	GameObject* obj = GetPlayer()->FindIncludeObject("Bullet");
	while (obj && obj->IsEntered()) {
		obj->Leave();
		obj = GetPlayer()->FindIncludeObject("Bullet");
	}

	//アニメーション・Aim
	Model::SetAllAnimeStop(true);
	GetPlayer()->GetAim()->SetAimMove(false);
}

void GameManager::EndPause()
{
	isPause_ = false;

	//全てのキャラクターのUpdateを許可
	CharacterManager::SetAllCharacterEnter();

	//全ての弾丸のUpdateを許可
	GameObject* obj = GetPlayer()->FindIncludeObject("Bullet");
	while (obj && !obj->IsEntered()) {
		obj->Enter();
		obj = GetPlayer()->FindIncludeObject("Bullet");
	}

	//アニメーション・Aim
	Model::SetAllAnimeStop(false);
	GetPlayer()->GetAim()->SetAimMove(true);
}

void GameManager::PauseClose()
{
	pauseClose_ = true;
}

bool GameManager::MouseLimitedChange()
{
	if (pauseClose_) {
		EndPause();
		pauseClose_ = false;
		isPause_ = false;
		return true;
	}

	//EscapeKeyで固定
	if (!Input::IsKeyDown(DIK_ESCAPE)) return false;

	//Pause開けるSceneじゃない時・エラーは終了
	SCENE_ID cs = GetSceneManager()->GetNextSceneID();
	if (cs == SCENE_ID_TITLE || cs == SCENE_ID_RESULT || !GameManager::GetStage()) return false;
	
	isPause_ = !isPause_;
	if (isPause_) StartPause();
	else EndPause();

	return true;
}

bool GameManager::IsMouseLimitedScene()
{
	//プレイシーンじゃない時はリミット無し
	SCENE_ID cs = GetSceneManager()->GetNextSceneID();
	if (cs == SCENE_ID_TITLE || cs == SCENE_ID_RESULT || !GameManager::GetStage()) return false;

	//プレイシーン中でPauseならリミット無し
	if (isPause_) return false;

	//プレイシーンでPauseじゃないならリミットあり
	return true;
}

EnemyManager* GameManager::GetEnemyManager() { return pEnemyManager_; }
NavigationAI* GameManager::GetNavigationAI() { return pNavigationAI_; }
WeaponObjectManager* GameManager::GetWeaponObjectManager() { return pWeaponObjectManager_; }
CreateStage* GameManager::GetCreateStage() { return pCreateStage_; }
SceneManager* GameManager::GetSceneManager() { return pSceneManager_; }
void GameManager::SetSceneManager(SceneManager* ma) { pSceneManager_ = ma; }
CollisionMap* GameManager::GetCollisionMap() { return pCollisionMap_; }
void GameManager::SetCollisionMap(CollisionMap* map) { pCollisionMap_ = map; }
Player* GameManager::GetPlayer() { return pPlayer_; }
void GameManager::SetPlayer(Player* player) { pPlayer_ = player; }
StageBase* GameManager::GetStage() { return pStage_; }
void GameManager::SetStage(StageBase* stage) { pStage_ = stage; }
