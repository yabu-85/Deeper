#include "EnemyBase.h"
#include "EnemyUi.h"
#include "DropTable.h"
#include "../GameManager/GameManager.h"
#include "../Player/Player.h"
#include "../Player/Aim.h"
#include "../State/StateManager.h"
#include "../Engine/Global.h"
#include "../VFXManager.h"

EnemyBase::EnemyBase(GameObject* parent, std::string name)
	: Character(parent, name), pEnemyUi_(nullptr), pStateManager_(nullptr), pCombatStateManager_(nullptr),
	type_(ENEMY_MAX), aimTargetPos_(0.0f), attackCoolDown_(0), hp_(0), maxHp_(0), attackDamage_(0), 
	combatDistance_(0.0f), isCombatReady_(false), attackDistance_(0.0f), actionCoolDown_(0), isAimTarget_(true)
{
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Update()
{
	int kcTime = GetKnockBackTime();
	if (kcTime > 0) {
		float speed = 1.0f - (1.0f / float(kcTime));
		SetKnockBackTime(--kcTime);
		if (speed > 0.0f) KnockBack(speed);
	}

	attackCoolDown_--;
	actionCoolDown_--;
	ReflectCharacter();
}

void EnemyBase::Release()
{
	SAFE_DELETE(pStateManager_);
	SAFE_DELETE(pCombatStateManager_);
	SAFE_DELETE(pEnemyUi_);
}

void EnemyBase::ApplyDamage(int da)
{
	hp_ -= da;
	
	if(pEnemyUi_) pEnemyUi_->SetParcent((float)(hp_) / (float)(maxHp_));
	
	if (hp_ <= 0) {
		StateManager* ma = GetStateManager();
		std::string name = ma->GetName();
		if(name != "Dead") GetStateManager()->ChangeState("Dead");
	}
}

void EnemyBase::DeadEnter()
{
	DropTable::DropItem(type_, transform_.position_);
	GetEnemyUi()->SetIsDraw(false);

	isAimTarget_ = false;
	GameManager::GetPlayer()->GetAim()->TargetIsDead(this);
}

void EnemyBase::Dead()
{
	VFXManager::CreatVfxSmoke(transform_.position_);
	GameManager::GetEnemyManager()->KillEnemy(this);
}

void EnemyBase::SetKnockBack(KNOCK_TYPE type, int time, float power, XMFLOAT3 pos)
{
	if (type == SMALL) SmallKnockBack();
	else if (type == MEDIUM) MediumKnockBack();
	else if (type == LARGE) LargetKnockBack();
	knockBackTime_ = time;
	knockBackPower_ = power;
	knockBackDirection_ = Float3Normalize(Float3Sub(transform_.position_, pos));
}

void EnemyBase::KnockBack(float speed)
{
	transform_.position_.x += (knockBackDirection_.x * speed);
	transform_.position_.z += (knockBackDirection_.z * speed);
	SetMovement(Float3Multiply(knockBackDirection_, speed));
}

bool EnemyBase::IsAttackReady()
{
	if (attackCoolDown_ <= 0) return true;
	return false;
}
