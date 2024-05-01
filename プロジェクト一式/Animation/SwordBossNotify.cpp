#include "SwordBossNotify.h"
#include "../Enemy/SwordBoss.h"
#include "../Action/MoveAction.h"
#include "../Action/RotateAction.h"
#include "../GameManager/GameManager.h"
#include "../Player/Player.h"

void SowrdBossAttackNotify::OnFrame(GameObject* obj)
{
	SwordBoss* boss = static_cast<SwordBoss*>(obj);
	boss->CalcPoly();
	boss->CalcCollider();
}

void SowrdBossAttackNotify::OnEnter(GameObject* obj)
{
	SwordBoss* boss = static_cast<SwordBoss*>(obj);
	boss->SetAllAttackColliderValid(true);
}

void SowrdBossAttackNotify::OnExit(GameObject* obj)
{
	SwordBoss* boss = static_cast<SwordBoss*>(obj);
	boss->AttackEnd();
}

void SowrdBossRotateNotify::OnFrame(GameObject* obj)
{
	SwordBoss* boss = static_cast<SwordBoss*>(obj);
	boss->GetRotateAction()->Update();
	boss->GetOrientedMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());
	boss->GetOrientedMoveAction()->Update();
}