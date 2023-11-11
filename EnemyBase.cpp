#include "EnemyBase.h"
#include "EnemyUi.h"
#include "GameManager.h"
#include "DropTable.h"
#include "EnemySpawnCtrl.h"

EnemyBase::EnemyBase(GameObject* parent)
	: GameObject(parent), pEnemyUi_(nullptr), hp_(0), maxHp_(0), type_(ENEMY_MAX), targetPos_(0.0f, 0.0f, 0.0f), state_(State::IDLE)
{
}

void EnemyBase::ApplyDamage(int da)
{
	hp_ -= da;
	pEnemyUi_->SetParcent((float)(hp_) / (float)(maxHp_));
	if (hp_ <= 0) {
		GameManager::GetDropTable()->DropItem(0, transform_.position_);
		GameManager::GetEnemySpawnCtrl()->KillEnemy(this);
	}
}
