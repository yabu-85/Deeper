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
#include "../Scene/StageBase.h"
#include "../Player/Player.h"
#include "../Animation/AnimationController.h"
#include "../Animation/SwordBossNotify.h"

namespace {
	static const int POLY_DRAW_TIME = 20;
	static const int POLY_SMOOTH = 1;
	static const float FIRST_LENG = 0.1f;
	static const float ADDLENG = 0.42f;
}

SwordBoss::SwordBoss(GameObject* parent)
	: EnemyBase(parent, "SwordBossEnemy"), hModel_(-1), hSwordModel_(-1), pMoveAction_(nullptr), pRotateAction_(nullptr), pOrientedMoveAction_(nullptr), 
	boneIndex_(-1), partIndex_(-1), polyCreatTime_(0), pDamageController_(nullptr), pAnimationController_(nullptr), pPolyLine_(nullptr),
	preRotate_{0,0,0}
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

	SetHP(300);
	SetMaxHP(300);
	SetBodyWeight(10.0f);
	SetBodyRange(0.5f);

	transform_.position_ = GameManager::GetStage()->GetBossPosition();
	type_ = ENEMY_SWORDBOSS;
	aimTargetPos_ = 1.2f;
	attackDistance_ = 2.5f;
	combatDistance_ = 5.0f;

	pEnemyUi_ = new EnemyUi(this);
	pEnemyUi_->Initialize(0.0f);
	pEnemyUi_->SetBssGauge();
	pEnemyUi_->SetAlphaMax();

	//アニメーションデータのセットフレームはヘッダに書いてる
	pAnimationController_ = new AnimationController(hModel_, this);
	for (int i = 0; i < (int)SWORDBOSS_ANIMATION::MAX; i++) pAnimationController_->AddAnim(SWORDBOSS_ANIMATION_DATA[i][0], SWORDBOSS_ANIMATION_DATA[i][1]);
	SowrdBossAttackNotify* notify = new SowrdBossAttackNotify(600, 690);
	SowrdBossRotateNotify* notifyRot = new SowrdBossRotateNotify(600, 690);
	pAnimationController_->AddAnimNotify((int)SWORDBOSS_ANIMATION::ATTACK1, notify);
	pAnimationController_->AddAnimNotify((int)SWORDBOSS_ANIMATION::ATTACK1, notifyRot);

	//Colliderの設定
	SphereCollider* collision1 = new SphereCollider(XMFLOAT3(0, 0.5f, 0), 0.35f);
	SphereCollider* collision2 = new SphereCollider(XMFLOAT3(0, 1.1f, 0), 0.35f);
	AddCollider(collision1);
	AddCollider(collision2);
	for (int i = 0; i < 4; i++) {
		SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 0.2f);
		AddAttackCollider(collision);
	}
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
	pStateManager_->Initialize();
	pStateManager_->ChangeState("Appear");

	//CombatStateの設定
	pCombatStateManager_ = new StateManager(this);
	pCombatStateManager_->AddState(new SwordBossWait(pCombatStateManager_));
	pCombatStateManager_->AddState(new SwordBossMove(pCombatStateManager_));
	pCombatStateManager_->AddState(new SwordBossAttack(pCombatStateManager_));
	pCombatStateManager_->Initialize();
	pCombatStateManager_->ChangeState("Wait");

	pDamageController_ = new DamageController;

	pPolyLine_ = new PolyLine;
	pPolyLine_->Load("PolyImage/Sword.png");
	pPolyLine_->SetLength(POLY_DRAW_TIME);
	pPolyLine_->SetSmooth(POLY_SMOOTH);
}

void SwordBoss::Update()
{
	OutputDebugStringA(std::to_string(CalculationDistance(GameManager::GetPlayer()->GetPosition(), transform_.position_)).c_str());
	OutputDebugString("\n\n");

	EnemyBase::Update();
	pAnimationController_->Update();
	pStateManager_->Update();	
	GameManager::GetCollisionMap()->CalcMapWall(transform_.position_, 0.1f, GetBodyRange());

	//ポリゴンを後ろから消してく
	if (polyCreatTime_ > 0) {
		polyCreatTime_--;
		pPolyLine_->ClearLastPositions();
	}

}

void SwordBoss::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	swordTransform_.position_ = Model::GetBoneAnimPosition(hModel_, boneIndex_, partIndex_);
	preRotate_ = swordTransform_.rotate_;
	swordTransform_.rotate_ = Model::GetBoneAnimRotate(hModel_, boneIndex_, partIndex_);
	swordTransform_.rotate_.y += transform_.rotate_.y;

	Model::SetTransform(hSwordModel_, swordTransform_);
	Model::Draw(hSwordModel_);
	
	CollisionDraw();

	//中間の攻撃コリジョンの表示をする
	XMFLOAT3 middle = Float3Add(swordTransform_.rotate_, Float3Multiply(Float3Sub(preRotate_, swordTransform_.rotate_), 0.5f));
	XMFLOAT3 target = CalculationDirection(middle);
	XMFLOAT3 pos = Float3Sub(swordTransform_.position_, transform_.position_);
	std::list<Collider*> list = GetAttackColliderList();
	float leng = FIRST_LENG;
	for (auto e : list) {
		XMFLOAT3 p = Float3Add(pos, Float3Multiply(target, leng));
		leng += ADDLENG;
		e->SetCenter(p);
		e->Draw(transform_.position_);
	}

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
	SAFE_DELETE(pOrientedMoveAction_);
	SAFE_DELETE(pAnimationController_);
	SAFE_DELETE(pDamageController_);

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
	float swordLeng = 1.7f;
	XMFLOAT3 target = CalculationDirection(swordTransform_.rotate_);
	pPolyLine_->AddPosition(swordTransform_.position_, Float3Add(swordTransform_.position_, Float3Multiply(target, swordLeng)));

	SetAllAttackColliderValid(true);
	std::list<Collider*> list = GetAttackColliderList();

	//中間の当たり判定
	XMFLOAT3 pos = Float3Sub(swordTransform_.position_, transform_.position_);
	float leng = FIRST_LENG;
	XMFLOAT3 middle = Float3Add(swordTransform_.rotate_, Float3Multiply(Float3Sub(preRotate_, swordTransform_.rotate_), 0.5f));
	target = CalculationDirection(middle);
	for (auto e : list) {
		XMFLOAT3 p = Float3Add(pos, Float3Multiply(target, leng));
		leng += ADDLENG;
		e->SetCenter(p);
		Collision(GameManager::GetPlayer());
	}

	//現在の当たり判定
	leng = 0.1f;
	target = CalculationDirection(swordTransform_.rotate_);
	for (auto e : list) {
		XMFLOAT3 p = Float3Add(pos, Float3Multiply(target, leng));
		leng += ADDLENG;
		e->SetCenter(p);
	}

}

void SwordBoss::AttackEnd()
{
	SetAllAttackColliderValid(false);
	GetDamageController()->ResetAttackList();
	SetPolyCreatTime();

}

void SwordBoss::SetPolyCreatTime()
{
	polyCreatTime_ = POLY_DRAW_TIME;
}
