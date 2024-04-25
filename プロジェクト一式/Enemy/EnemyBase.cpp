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
	type_(ENEMY_MAX), aimTargetPos_(0.0f), attackCoolDown_(0), combatDistance_(0.0f), isCombatReady_(false), 
	attackDistance_(0.0f), actionCoolDown_(0), isAimTarget_(true)
{
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Update()
{
	Character::Update();

	attackCoolDown_--;
	actionCoolDown_--;
}

void EnemyBase::Release()
{
	SAFE_DELETE(pStateManager_);
	SAFE_DELETE(pCombatStateManager_);
	SAFE_DELETE(pEnemyUi_);
}

void EnemyBase::DeadEnter()
{
	SetAllColliderValid(false);
	SetAllAttackColliderValid(false);

	DropTable::DropItem(type_, transform_.position_);
	GetEnemyUi()->SetIsDraw(false);

	isAimTarget_ = false;
	GameManager::GetPlayer()->GetAim()->TargetIsDead(this);
}

void EnemyBase::DeadExit()
{
	VFXManager::CreatVfxSmoke(transform_.position_);
	GameManager::GetEnemyManager()->KillEnemy(this);
}

bool EnemyBase::IsAttackReady()
{
	if (attackCoolDown_ <= 0) return true;
	return false;
}

void EnemyBase::Dead()
{
	if (GetStateManager()->GetName() != "Dead") GetStateManager()->ChangeState("Dead");
}

void EnemyBase::Damage()
{
	if (pEnemyUi_) pEnemyUi_->SetParcent((float)(GetHP()) / (float)(GetMaxHP()));

	if (pStateManager_->GetName() == "Dead") return;

	if (pStateManager_->GetName() != "Combat") {
		pStateManager_->ChangeState("Combat");
	}
}



EnemyFrame::EnemyFrame(int start, int end)
	: pParent_(nullptr), frame(start, end), inFrame_(false)
{
}

EnemyAttackController::~EnemyAttackController()
{
	for (EnemyFrame* action : actions) delete action;
	actions.clear();
}

void EnemyAttackController::Update(int currentTime)
{
	for (EnemyFrame* action : actions) {
		//フレーム内・Enterの判定
		if (action->IsTargetFrame(currentTime)) {
			if (!action->GetInFrame()) {
				action->OnEnter();
				action->SetInFrame(true);
			}
			action->Update();
		}

		//終了フレームかどうか専用判定
		if (action->IsEndFrame(currentTime)) {
			action->OnExit();
			action->SetInFrame(false);
		}
	}
}