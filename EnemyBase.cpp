#include "EnemyBase.h"
#include "EnemyUi.h"
#include "GameManager.h"
#include "DropTable.h"
#include "Player.h"
#include "StateManager.h"
#include "GameManager.h"
#include "DamageManager.h"
#include "Engine/Global.h"

EnemyBase::EnemyBase(GameObject* parent)
	: Character(parent), pEnemyUi_(nullptr), pStateManager_(nullptr), pCombatStateManager_(nullptr),
	type_(ENEMY_MAX), aimTargetPos_(0.0f)
{
	GameManager::GetDamageManager()->AddCharacter(this, DamageManager::DamageType::DA_Enemy);
}

EnemyBase::~EnemyBase()
{
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
		GameManager::GetDropTable()->DropItem(0, transform_.position_);
		GameManager::GetEnemyManager()->KillEnemy(this);
	}
}
