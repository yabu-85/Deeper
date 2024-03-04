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

void EnemyBase::SetKnockBack(KNOCK_TYPE type, int time)
{
	if (type == SMALL) SmallKnockBack();
	else if (type == MEDIUM) MediumKnockBack();
	else if (type == LARGE) LargetKnockBack();
}

void EnemyBase::KnockBack(float speed)
{
	XMVECTOR vMove = { 0.0, 0.0, -1.0, 0.0 };
	XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	vMove = XMVector3TransformCoord(vMove, mRotY);
	vMove = XMVector3Normalize(vMove) * (speed + 0.1f);
	XMFLOAT3 move{};
	XMStoreFloat3(&move, vMove);

	transform_.position_.x += move.x;
	transform_.position_.z += move.z;
	SetMovement(XMLoadFloat3(&move));
}

bool EnemyBase::IsAttackReady()
{
	if (attackCoolDown_ <= 0) return true;
	return false;
}
