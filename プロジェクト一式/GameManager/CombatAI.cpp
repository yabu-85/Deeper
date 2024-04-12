#include "CombatAI.h"
#include "GameManager.h"
#include "DifficultyManager.h"
#include "../Player/Player.h"
#include "../Player/Aim.h"
#include "../Enemy/EnemyBase.h"
#include "../Enemy/EnemyManager.h"
#include "../State/StateManager.h"
#include "../Engine/Global.h"
#include "../Engine/Camera.h"
#include <vector>
#include <queue>

using std::vector;
using std::priority_queue;

namespace CombatAI {
	bool compare(EnemyBase* e1, EnemyBase* e2) {
		XMFLOAT3 plaPos = GameManager::GetPlayer()->GetPosition();
		float dist1 = CalculationDistance(e1->GetPosition(), plaPos);
		float dist2 = CalculationDistance(e2->GetPosition(), plaPos);
		return dist1 < dist2;
	}

	unsigned calcTime_ = 0;
	static const unsigned CALC_RAND = 10;

	//設計を見直していく
	//考えられる大枠の条件
	//MoveとAttack

	//エネミー事に条件をかく？

	//ガード型と戦っていたら、付近の他エネミーが攻撃しに行くのが頭いい
	//ガード型がいるとして、他のエネミーが攻撃されていたら守りに入るよね
}

void CombatAI::Initialize()
{
}

void CombatAI::Update() {
	//CALC_RANDの時間になったら計算する
	if (CALC_RAND > calcTime_++) return;
	calcTime_ = 0;

	//プレイヤーと比較して近い順のリスト
	vector<EnemyBase*> eList = GameManager::GetEnemyManager()->GetAllEnemy();
	if (eList.empty()) return;
	std::sort(eList.begin(), eList.end(), compare);

	//個数求める
	int random = rand() % 100;
	int count = 0;
	if (random > 80) count = 2;
	else if (random > 40) count = 1;
	else if (random > 0) count = 0;

	//エネミーの数より多いなら抑制
	if (eList.size() < count) count = (int)eList.size();

	//近い数分フラグをセット
	for (int i = 0; i < count; i++) {
		eList[i]->SetCombatReady(true);
	}
	
}

bool CombatAI::IsEnemyAttackPermission(EnemyBase* enemy)
{
	return (DifficultyManager::AttackPermission() && enemy->GetCombatReady());
}

bool CombatAI::IsEnemyMovePermission(EnemyBase* enemy)
{
	//修正箇所 //とりあえずのrand
	if (enemy == GameManager::GetPlayer()->GetAim()->GetTargetEnemy()) return true;

	if (rand() % 3 == 0) return true;
	return false;
}