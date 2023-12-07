#include "EnemyBase.h"
#include "EnemyUi.h"
#include "GameManager.h"
#include "DropTable.h"
#include "EnemySpawnCtrl.h"
#include "Player.h"
#include "StateManager.h"
#include "GameManager.h"
#include "DamageCtrl.h"

EnemyBase::EnemyBase(GameObject* parent)
	: Character(parent), pEnemyUi_(nullptr), pStateManager_(nullptr), pCombatStateManager_(nullptr),
	type_(ENEMY_MAX), prePos_(0.0f, 0.0f, 0.0f), aimTargetPos_(0.0f)
{
	GameManager::GetDamageCtrl()->AddCharacter(this, DamageCtrl::DamageType::DA_Enemy);
}

EnemyBase::~EnemyBase()
{
	delete pStateManager_;
	delete pEnemyUi_;
}

void EnemyBase::ApplyDamage(int da)
{
	hp_ -= da;
	
	if(pEnemyUi_) pEnemyUi_->SetParcent((float)(hp_) / (float)(maxHp_));
	
	if (hp_ <= 0) {
		GameManager::GetDropTable()->DropItem(0, transform_.position_);
		GameManager::GetEnemySpawnCtrl()->KillEnemy(this);
	}
}
