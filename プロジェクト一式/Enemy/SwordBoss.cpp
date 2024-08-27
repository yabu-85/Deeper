#include "SwordBoss.h"
#include "EnemyUi.h"
#include "SwordBossAttacks.h"
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
#include "../Other/VFXManager.h"

namespace {
	static const int POLY_DRAW_TIME = 20;
	static const int POLY_SMOOTH = 0;
	static const float POLY_LENG = 1.7f;

	static const int ATTACK_COLLIDER = 5;	//コライダーの数
	static const float FIRST_LENG = 0.03f;	//コライダーの最初のポジション
	static const float ADDLENG = 0.36f;		//コライダーのポジションに足す量

}

SwordBoss::SwordBoss(GameObject* parent)
	: EnemyBase(parent, "SwordBossEnemy"), hModel_(-1), hSwordModel_(-1), pAstarMoveAction_(nullptr), pRotateAction_(nullptr), pOrientedMoveAction_(nullptr),
	boneIndex_(-1), partIndex_(-1), pDamageController_(nullptr), pAnimationController_(nullptr), pDoublePolyLine_(nullptr), pSelectAttack_(nullptr),
	preRotate_{0,0,0}, prePosition_{0,0,0}
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
	SetBodyRange(0.7f);

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
	//AnimNotify Slash_Up
	pAnimationController_->AddAnimNotify((int)SWORDBOSS_ANIMATION::Slash_Up, new SowrdBossAttackNotify(455, 473));
	pAnimationController_->AddAnimNotify((int)SWORDBOSS_ANIMATION::Slash_Up, new SowrdBossRotateNotify(420, 455));
	pAnimationController_->AddAnimNotify((int)SWORDBOSS_ANIMATION::Slash_Up, new SowrdBossVfxNotify(430, 475));
	//AnimNotify Slash_Right
	pAnimationController_->AddAnimNotify((int)SWORDBOSS_ANIMATION::Slash_Right, new SowrdBossAttackNotify(560, 575));
	pAnimationController_->AddAnimNotify((int)SWORDBOSS_ANIMATION::Slash_Right, new SowrdBossRotateNotify(520, 560));
	pAnimationController_->AddAnimNotify((int)SWORDBOSS_ANIMATION::Slash_Right, new SowrdBossVfxNotify(520, 575));
	//AnimNotify Slash_Jump
	pAnimationController_->AddAnimNotify((int)SWORDBOSS_ANIMATION::Slash_Jump, new SowrdBossAttackNotify(760, 780));
	pAnimationController_->AddAnimNotify((int)SWORDBOSS_ANIMATION::Slash_Jump, new SowrdBossRotateNotify(710, 765));
	pAnimationController_->AddAnimNotify((int)SWORDBOSS_ANIMATION::Slash_Jump, new SowrdBossVfxNotify(720, 780));
	//AnimNotify Thrust
	pAnimationController_->AddAnimNotify((int)SWORDBOSS_ANIMATION::Thrust, new SowrdBossAttackNotify(650, 662));
	pAnimationController_->AddAnimNotify((int)SWORDBOSS_ANIMATION::Thrust, new SowrdBossRotateNotify(600, 650));
	pAnimationController_->AddAnimNotify((int)SWORDBOSS_ANIMATION::Thrust, new SowrdBossVfxNotify(620, 662));

	//Colliderの設定
	AddCollider(new SphereCollider(XMFLOAT3(0, 0.5f, 0), 0.35f));
	AddCollider(new SphereCollider(XMFLOAT3(0, 1.1f, 0), 0.35f));
	//AttackCollider
	for (int i = 0; i < ATTACK_COLLIDER; i++) AddAttackCollider(new SphereCollider(XMFLOAT3(0, 0, 0), 0.2f));
	SetAllAttackColliderValid(false);

	//Actionの設定
	pAstarMoveAction_ = new AstarMoveAction(this, 0.0f, 0.3f);
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

	//技ごとに変更するように todo
	pDamageController_ = new DamageController;
	DamageInfo damage(this, "SwordBoss", 12);
	KnockBackInfo knockBack(KNOCK_TYPE::MEDIUM, 40, 0.01f, transform_.position_);
	pDamageController_->SetCurrentDamage(damage);
	pDamageController_->SetCurrentKnockBackInfo(knockBack);

	pSelectAttack_ = new SelectAttack;
	pSelectAttack_->AddSelectAttack(&SwordBossSlashUp::singleton());
	pSelectAttack_->AddSelectAttack(&SwordBossSlashRight::singleton());
	pSelectAttack_->AddSelectAttack(&SwordBossSlashJump::singleton());
	pSelectAttack_->AddSelectAttack(&SwordBossThrust::singleton());

	pDoublePolyLine_ = new DoublePolyLine;
	pDoublePolyLine_->Load("PolyImage/BossSword.png");
	pDoublePolyLine_->SetLength(POLY_DRAW_TIME);
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

	prePosition_ = swordTransform_.position_;
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
		e->SetCenter(p);
		e->Draw(transform_.position_);
		leng += ADDLENG;
	}

	pEnemyUi_->Draw();
	pAstarMoveAction_->Draw();
	pDoublePolyLine_->Draw();
}

void SwordBoss::Release()
{
	SAFE_DELETE(pSelectAttack_);
	
	SAFE_RELEASE(pDoublePolyLine_);
	SAFE_DELETE(pDoublePolyLine_);

	SAFE_DELETE(pRotateAction_);
	SAFE_DELETE(pAstarMoveAction_);
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

void SwordBoss::CreateVfxSword()
{
	std::list<Collider*> list = GetAttackColliderList();
	XMFLOAT3 vfxDir = Float3Sub(swordTransform_.position_, prePosition_);
	for (auto e : list) {
		XMFLOAT3 pos = Float3Add(transform_.position_, e->GetCenter());
		VFXManager::CreateVfxBossSword(pos, vfxDir);
	}
}

void SwordBoss::CalcPoly()
{
	XMFLOAT3 target = CalculationDirection(swordTransform_.rotate_);
	pDoublePolyLine_->AddPosition(swordTransform_.position_, Float3Add(swordTransform_.position_, Float3Multiply(target, POLY_LENG)));
}

void SwordBoss::CalcCollider()
{
	//まず中間の当たり判定
	std::list<Collider*> list = GetAttackColliderList();
	XMFLOAT3 pos = Float3Sub(swordTransform_.position_, transform_.position_);
	XMFLOAT3 middle = Float3Add(swordTransform_.rotate_, Float3Multiply(Float3Sub(preRotate_, swordTransform_.rotate_), 0.5f));
	XMFLOAT3 target = CalculationDirection(middle);
	float leng = FIRST_LENG;

	for (auto e : list) {
		XMFLOAT3 p = Float3Add(pos, Float3Multiply(target, leng));
		e->SetCenter(p);
		leng += ADDLENG;
	}
	Collision(GameManager::GetPlayer());

	//現在の当たり判定
	leng = FIRST_LENG;
	target = CalculationDirection(swordTransform_.rotate_);
	for (auto e : list) {
		e->SetCenter(Float3Add(pos, Float3Multiply(target, leng)));
		leng += ADDLENG;
	}
}

void SwordBoss::AttackEnd()
{
	SetAllAttackColliderValid(false);
	GetDamageController()->ResetAttackList();
	pDoublePolyLine_->SetClear(true);
}

void SwordBoss::SetDamageInfo1()
{
}

void SwordBoss::SetDamageInfo2()
{
}

void SwordBoss::SetDamageInfo3()
{
}