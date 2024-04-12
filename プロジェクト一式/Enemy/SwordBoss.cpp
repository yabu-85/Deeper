#include "SwordBoss.h"
#include "EnemyUi.h"
#include "../GameManager/GameManager.h"
#include "../Engine/Model.h"
#include "../Engine/SphereCollider.h"
#include "../Engine/Global.h"
#include "../Engine/PolyLine.h"
#include "../State/StateManager.h"
#include "../State/SwordBossState.h"
#include "../Stage/CreateStage.h"
#include "../Stage/CollisionMap.h"

#include "../Action/MoveAction.h"
#include "../Action/RotateAction.h"

SwordBoss::SwordBoss(GameObject* parent)
	: EnemyBase(parent, "SwordBossEnemy"), hModel_(-1), hSwordModel_(-1), pMoveAction_(nullptr), pRotateAction_(nullptr), pOrientedMoveAction_(nullptr), 
	boneIndex_(-1), partIndex_(-1), pDamageController_(nullptr), pAnimationController_(nullptr), pPolyLine_(nullptr)
{
}

SwordBoss::~SwordBoss()
{
}

void SwordBoss::Initialize()
{
	hModel_ = Model::Load("Model/SwordBoss.fbx");
	assert(hModel_ >= 0);
	hSwordModel_ = Model::Load("Model/ThickSword.fbx");
	assert(hSwordModel_ >= 0);

	Model::GetBoneIndex(hModel_, "Weapon", &boneIndex_, &partIndex_);
	assert(boneIndex_ >= 0);

	transform_.position_ = { 10.0f, 0.0f, 10.0f };
	transform_.rotate_.y = (float)(rand() % 360);

	SetHP(300);
	SetMaxHP(300);
	SetBodyWeight(10.0f);
	SetBodyRange(0.5f);

	type_ = ENEMY_SWORDBOSS;
	aimTargetPos_ = 1.2f;
	attackDistance_ = 2.0f;
	combatDistance_ = 5.0f;

	pEnemyUi_ = new EnemyUi(this);
	pEnemyUi_->Initialize(0.0f);
	pEnemyUi_->SetBssGauge();
	pEnemyUi_->SetAlphaMax();

	//アニメーションデータのセットフレームはヘッダに書いてる
	pAnimationController_ = new AnimationController(hModel_);
	for (int i = 0; i < (int)SWORDBOSS_ANIMATION::MAX; i++) pAnimationController_->AddAnime(SWORDBOSS_ANIMATION_DATA[i].startFrame, SWORDBOSS_ANIMATION_DATA[i].endFrame);
	Model::SetAnimFrame(hModel_, 0, 900, 1.0f);

	//Colliderの設定
	SphereCollider* collision1 = new SphereCollider(XMFLOAT3(0, 0.5f, 0), 0.35f);
	SphereCollider* collision2 = new SphereCollider(XMFLOAT3(0, 1.1f, 0), 0.35f);
	SphereCollider* collision3 = new SphereCollider(XMFLOAT3(0, 0, 0), 0.2f);
	SphereCollider* collision4 = new SphereCollider(XMFLOAT3(0, 0, 0), 0.2f);
	SphereCollider* collision5 = new SphereCollider(XMFLOAT3(0, 0, 0), 0.2f);
	AddCollider(collision1);
	AddCollider(collision2);
	AddAttackCollider(collision3);
	AddAttackCollider(collision4);
	AddAttackCollider(collision5);
	SetAllAttackColliderValid(false);

	//Actionの設定
	pMoveAction_ = new AstarMoveAction(this, 0.0f, 0.3f);
	pOrientedMoveAction_ = new OrientedMoveAction(this, 0.02f);
	pRotateAction_ = new RotateAction(this, 0.0f);
	pRotateAction_->Initialize();

	//ステートの設定
	pStateManager_ = new StateManager(this);
	pStateManager_->AddState(new SwordBossAppear(pStateManager_));
	pStateManager_->AddState(new SwordBossDead(pStateManager_));
	pStateManager_->AddState(new SwordBossCombat(pStateManager_));
	pStateManager_->ChangeState("Appear");
	pStateManager_->Initialize();

	//CombatStateの設定
	pCombatStateManager_ = new StateManager(this);
	pCombatStateManager_->AddState(new SwordBossWait(pCombatStateManager_));
	pCombatStateManager_->AddState(new SwordBossMove(pCombatStateManager_));
	pCombatStateManager_->AddState(new SwordBossAttack(pCombatStateManager_));
	pCombatStateManager_->ChangeState("Wait");
	pCombatStateManager_->Initialize();

	pDamageController_ = new DamageController;
	pPolyLine_ = new PolyLine;
	pPolyLine_->Load("PolyImage/Sword.png");
	pPolyLine_->SetLength(30);
	pPolyLine_->SetSmooth(0);
}

void SwordBoss::Update()
{
	EnemyBase::Update();
	pAnimationController_->Update();
	pStateManager_->Update();	
	GameManager::GetCollisionMap()->CalcMapWall(transform_.position_, 0.1f, GetBodyRange());
}

void SwordBoss::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	swordTransform_.position_ = Model::GetBoneAnimPosition(hModel_, boneIndex_, partIndex_);
	swordTransform_.rotate_ = Model::GetBoneAnimRotate(hModel_, boneIndex_, partIndex_);
	swordTransform_.rotate_.y += transform_.rotate_.y;
	Model::SetTransform(hSwordModel_, swordTransform_);
	Model::Draw(hSwordModel_);
	
	CalcPoly();
	CollisionDraw();

	pEnemyUi_->Draw();
	pMoveAction_->Draw();
	pPolyLine_->Draw();
}

void SwordBoss::Release()
{
	SAFE_RELEASE(pPolyLine_);
	SAFE_DELETE(pPolyLine_);

	SAFE_DELETE(pRotateAction_);
	SAFE_DELETE(pMoveAction_);

	EnemyBase::Release();
	Model::Release(hModel_);

}

void SwordBoss::OnAttackCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player") {
		Character* p = static_cast<Character*>(pTarget);
		DamageInfo damage(GetDamageController()->GetCurrentDamage());
		KnockBackInfo knock(GetDamageController()->GetCurrentKnockBackInfo());
		knock.pos = this->GetPosition();

		//攻撃入ったらリストに追加
		if (p->ApplyDamageWithList(damage, knock)) {
			GetDamageController()->AddAttackList(p);
		}
	}
}

void SwordBoss::CalcPoly()
{
	float swordLeng = 1.5f;
	XMFLOAT3 target = CalculationDirection(swordTransform_.rotate_);
	pPolyLine_->AddPosition(swordTransform_.position_, Float3Add(swordTransform_.position_, Float3Multiply(target, swordLeng)));

	std::list<Collider*> list = GetAttackColliderList();
	auto it = list.begin();
	float leng = 0.4f;
	float addLeng = 0.5f;
	int size = (int)list.size();
	for (int i = 0; i < size; i++) {
		XMFLOAT3 pos = Float3Sub(swordTransform_.position_, transform_.position_);
		pos = Float3Add(pos, Float3Multiply(target, leng));
		(*it)->SetCenter(pos);
		(*it)->SetValid(true);
		it++;
		leng += addLeng;
	}

}

void SwordBoss::DamageInfoReset()
{
	SetAllAttackColliderValid(false);
	GetDamageController()->ResetAttackList();
	pPolyLine_->ResetPosition();
}

void SwordBoss::SetDamageInfoCombo1()
{
	DamageInfo damage(this, "MeleeAttack", 3);
	KnockBackInfo knockBack(KNOCK_TYPE::MEDIUM, 0, 0.0f, transform_.position_);
	GetDamageController()->SetCurrentDamage(damage);
	GetDamageController()->SetCurrentKnockBackInfo(knockBack);
	SetAllAttackColliderValid(true);
}