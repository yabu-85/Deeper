#include "EnemyBase.h"
#include "EnemyUi.h"
#include "GameManager.h"
#include "DropTable.h"
#include "EnemySpawnCtrl.h"

EnemyBase::EnemyBase(GameObject* parent)
	: GameObject(parent), pEnemyUi_(nullptr), hp_(0), maxHp_(0), type_(ENEMY_MAX), pStateManager_(nullptr)
{
}

void EnemyBase::ApplyDamage(int da)
{
	hp_ -= da;
	pEnemyUi_->SetParcent((float)(hp_) / (float)(maxHp_));
	if (hp_ <= 0) {
		GameManager* gm = (GameManager*)FindObject("GameManager");
		gm->GetDropTable()->DropItem(0, transform_.position_);
		gm->GetEnemySpawnCtrl()->KillEnemy(this);
	}
}
