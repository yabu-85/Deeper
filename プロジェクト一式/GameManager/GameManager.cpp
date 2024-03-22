#include "GameManager.h"
#include "WaveManager.h"
#include "CombatAI.h"
#include "DifficultyManager.h"
#include "../VFXManager.h"
#include "../Enemy/EnemyManager.h"
#include "../Stage/NavigationAI.h"
#include "../Stage/CreateStage.h"
#include "../Stage/CollisionMap.h"
#include "../Weapon/WeaponObjectManager.h"
#include "../Engine/TransitionEffect.h"
#include "../Engine/GameObject.h"
#include "../Engine/Model.h"
#include "../Engine/SceneManager.h"
#include "../Player/LifeManager.h"
#include "../Player/Player.h"
#include "../Player/PlayerData.h"
#include "../UI/Interaction.h"
#include "../Scene/SceneBase.h"

//Pause
#include "../Engine/Input.h"
#include "../Character/CharacterManager.h"
#include "../Scene/StageBase.h"
#include "../Player/Aim.h"
#include "../AudioManager.h"

//�f�o�b�O�p
#include "../Enemy/EnemyBase.h"

namespace GameManager {
	void MouseLimitedChange();
	void StartPause();
	void EndPause();

	bool isPause_ = false;		//Pause��Ԃ��ǂ���
	bool pauseClose_ = false;	//Key�ŕ���p�̃t���O

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
	//�����^�C�g���V�[���E���U���g�V�[���ł�Update�͂���Ȃ���Draw����
	if (!GameManager::GetStage()) return;

	//Pause���Ȃ�UI��Update
	if (isPause_) GetStage()->UIUpdate();

	MouseLimitedChange();

	DifficultyManager::Update();
	CombatAI::Update();
	WaveManager::Update();
	LifeManager::Update();

	//�f�o�b�O�p
	if (GetStage()) {
		if (Input::IsKeyDown(DIK_M)) { pEnemyManager_->SpawnEnemy(ENEMY_STONEGOLEM); }
		if (Input::IsKeyDown(DIK_N)) { for (int i = 0; i < 20; i++) pEnemyManager_->SpawnEnemy(ENEMY_STONEGOLEM); }
		if (Input::IsKeyDown(DIK_L)) { pEnemyManager_->SpawnEnemy(ENEMY_THROW); }
		if (Input::IsKeyDown(DIK_K)) { for (int i = 0; i < 20; i++) pEnemyManager_->SpawnEnemy(ENEMY_THROW); }
		if (Input::IsKeyDown(DIK_P)) { pEnemyManager_->SpawnEnemy(ENEMY_MELEE); }
		if (Input::IsKeyDown(DIK_O)) { for (int i = 0; i < 20; i++) pEnemyManager_->SpawnEnemy(ENEMY_MELEE); }

		if (Input::IsKeyDown(DIK_B)) pWeaponObjectManager_->AllKillWeaponObject();
		if (Input::IsKeyDown(DIK_V)) {
			std::vector<EnemyBase*> eList = pEnemyManager_->GetAllEnemy();
			DamageInfo damage(10000000);
			for (auto e : eList) e->ApplyDamageDirectly(damage);
		}
		if (Input::IsKey(DIK_C)) {
			std::vector<EnemyBase*> eList = pEnemyManager_->GetAllEnemy();
			DamageInfo damage(1);
			for (auto e : eList) e->ApplyDamageDirectly(damage);
		}
		if (Input::IsKeyDown(DIK_TAB)) {
			OutputDebugString("entity : ");
			int count = (int)pEnemyManager_->GetAllEnemy().size();
			OutputDebugStringA(std::to_string(count).c_str());
			OutputDebugString("\n");
		}
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

		//Pause���Ȃ�UI��Draw
		if (isPause_) GetStage()->UIDraw();

	}

	TransitionEffect::Draw();

}

void GameManager::SceneTransitionInitialize()
{
	pauseClose_ = false;
	isPause_ = false;

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
}

void GameManager::StartPause()
{
	pauseClose_ = false;
	isPause_ = true;
	AudioManager::Play(AUDIO_ID::PAUSE_OPEN);

	//�S�ẴL�����N�^�[��Update������
	CharacterManager::SetAllCharacterLeave();

	//�S�Ă̒e�ۂ�Update������
	GameObject* obj = GetPlayer()->FindIncludeObject("Bullet");
	while (obj && obj->IsEntered()) {
		obj->Leave();
		obj = GetPlayer()->FindIncludeObject("Bullet");
	}

	//�A�j���[�V�����EAim
	Model::SetAllAnimeStop(true);
	GetPlayer()->GetAim()->SetValid(false);
}

void GameManager::EndPause()
{
	pauseClose_ = false;
	isPause_ = false;

	//�S�ẴL�����N�^�[��Update������
	CharacterManager::SetAllCharacterEnter();

	//�S�Ă̒e�ۂ�Update������
	GameObject* obj = GetPlayer()->FindIncludeObject("Bullet");
	while (obj && !obj->IsEntered()) {
		obj->Enter();
		obj = GetPlayer()->FindIncludeObject("Bullet");
	}

	//�A�j���[�V�����EAim
	Model::SetAllAnimeStop(false);
	GetPlayer()->GetAim()->SetValid(true);
}

void GameManager::PauseClose()
{
	pauseClose_ = true;
}

void GameManager::MouseLimitedChange()
{
	//Pause�J����Scene����Ȃ����E�G���[�͏I��
	SCENE_ID cs = GetSceneManager()->GetNextSceneID();
	if (cs == SCENE_ID_TITLE || cs == SCENE_ID_RESULT || !GameManager::GetStage()) return;

	//pauseClose�t���Otrue�Ȃ����
	if (pauseClose_) {
		EndPause();
		return;
	}

	//EscapeKey�ŌŒ�ŉ����ĂȂ��Ȃ�I���
	if (!Input::IsKeyDown(DIK_ESCAPE)) return;

	//�|�[�Y����Escape�������������
	if (isPause_) {
		AudioManager::Play(AUDIO_ID::PAUSE_CLOSE);
		EndPause();
		return;
	}

	isPause_ = !isPause_;
	if (isPause_) StartPause();
	else EndPause();
}

bool GameManager::IsMouseLimitedScene()
{
	//�v���C�V�[������Ȃ����̓��~�b�g����
	SCENE_ID cs = GetSceneManager()->GetNextSceneID();
	if (cs == SCENE_ID_TITLE || cs == SCENE_ID_RESULT || !GameManager::GetStage()) return false;

	//�v���C�V�[������Pause�Ȃ烊�~�b�g����
	if (isPause_) return false;

	//�v���C�V�[����Pause����Ȃ��Ȃ烊�~�b�g����
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
