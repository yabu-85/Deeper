#include "EnemyBase.h"
#include "EnemyUi.h"
#include "../GameManager.h"
#include "DropTable.h"
#include "../Player/Player.h"
#include "../Player/Aim.h"
#include "../State/StateManager.h"
#include "../GameManager.h"
#include "../Engine/Global.h"

EnemyBase::EnemyBase(GameObject* parent, std::string name)
	: Character(parent, name), pEnemyUi_(nullptr), pStateManager_(nullptr), pCombatStateManager_(nullptr),
	type_(ENEMY_MAX), aimTargetPos_(0.0f), attackCoolDown_(0), hp_(0), maxHp_(0), attackDamage_(0), 
	combatDistance_(0.0f), isCombatReady_(false), attackDistance_(0.0f), actionCoolDown_(0)
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
	//AimのTargetを更新する関数
	GameManager::GetPlayer()->GetAim()->TargetIsDead(this);

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

void EnemyBase::Dead()
{
	DropTable::DropItem(type_, transform_.position_);
	GameManager::GetEnemyManager()->KillEnemy(this);
}

void EnemyBase::KnockBack(KNOCK_TYPE type)
{
	if (type == SMALL) SmallKnockBack();
	else if (type == MEDIUM) MediumKnockBack();
	else if (type == LARGE) LargetKnockBack();
}

bool EnemyBase::IsAttackReady()
{
	if (attackCoolDown_ <= 0) return true;
	return false;
}
