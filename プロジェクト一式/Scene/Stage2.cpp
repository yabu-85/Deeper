#include "Stage2.h"
#include "../Other/AudioManager.h"
#include "../Engine/TransitionEffect.h"
#include "../GameManager/GameManager.h"
#include "../Stage/CreateStage.h"
#include "../Stage/Warp.h"
#include "../Enemy/EnemyManager.h"
#include "../Player/Player.h"
#include "../Player/Aim.h"

//�f�o�b�O�p
#include "../Weapon/WeaponObjectManager.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Input.h"

namespace {
	//Warp�̏��
	static const XMFLOAT3 COMPULSION_POS = { 0.0f, 5.0f, 0.0f };	//Offset�ʒu
	static const float COMPULSION_DISTANCE = 5.0f;					//�ǂ̂��炢��납��\�����邩
	static const int COMPULSION_TIME = 120;							//�Œ莞��

}

Stage2::Stage2(GameObject* parent)
	: StageBase(parent, "Stage2"), compTime_(0)
{
}

void Stage2::Initialize()
{
	AudioManager::Initialize();
	TransitionEffect::SetFade(TRANSITION_TYPE::TYPE_ALPHA);
	TransitionEffect::SetAlphaDecrease(0.01f);

	InitializeStage("Csv/Map2.csv", "Model/Stage/SkyBox.fbx");
	SetWarpStage(SCENE_ID_STAGE3);
	GameManager::GetEnemyManager()->SpawnEnemyTable(ETABLE_NORMAL);

	//�f�o�b�O�p
	GameManager::GetWeaponObjectManager()->AddWeaponObject(ENEMY_TYPE::ENEMY_STONEGOLEM, GameManager::GetStage()->GetStartPosition());

}

void Stage2::Update()
{
	if (IsClearStage()) {
		OnStageCleared();
	}

	if (compTime_ > 0) {
		compTime_--;
		Player* p = GameManager::GetPlayer();
		XMFLOAT3 pos = Float3Add(p->GetPosition(), COMPULSION_POS);
		XMFLOAT3 vec = Float3Sub(p->GetPosition(), Float3Add(GetWarp()->GetPosition(), XMFLOAT3(0.5f, 0.0f, 0.5f)));
		vec = Float3Multiply(Float3Normalize(vec), COMPULSION_DISTANCE);
		pos = Float3Add(pos, vec);
		p->GetAim()->SetCompulsion(pos, GetWarp()->GetPosition());
	
	}

}

void Stage2::Draw()
{
	GameManager::GetCreateStage()->Draw();

}

void Stage2::Release()
{
	AudioManager::Release();

}

void Stage2::OnStageCleared()
{
	SetWarpValid(true);
	compTime_ = COMPULSION_TIME;
}
