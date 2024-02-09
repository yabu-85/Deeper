#include "EnemyBase.h"
#include "EnemyUi.h"
#include "../GameManager.h"
#include "DropTable.h"
#include "../Player/Player.h"
#include "../State/StateManager.h"
#include "../GameManager.h"
#include "../Engine/Global.h"

EnemyBase::EnemyBase(GameObject* parent, std::string name)
	: Character(parent, name), pEnemyUi_(nullptr), pStateManager_(nullptr), pCombatStateManager_(nullptr),
	type_(ENEMY_MAX), aimTargetPos_(0.0f), attackCoolDown_(0), hp_(0), maxHp_(0), attackDamage_(0)
{
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Update()
{
	attackCoolDown_--;
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
		DropTable::DropItem(type_, transform_.position_);
		GameManager::GetEnemyManager()->KillEnemy(this);
	}
}

bool EnemyBase::IsAttackReady()
{
	if (attackCoolDown_ <= 0) return true;
	return false;
}
