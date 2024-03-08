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
	float FuzzyGrade(float value, float x0, float x1) {
		//エラー
		if (x0 <= x1) return 0.0f;
		if (value < x0) return 0.0f;
		if (value >= x1) return 1.0f;
		if (x0 <= value && value < x1) return (value - x0) / (x1 - x0);
		return 0.0f;
	}

	bool compare(EnemyBase* e1, EnemyBase* e2) {
		XMFLOAT3 plaPos = GameManager::GetPlayer()->GetPosition();
		float dist1 = DistanceCalculation(e1->GetPosition(), plaPos);
		float dist2 = DistanceCalculation(e2->GetPosition(), plaPos);
		return dist1 < dist2;
	}

	bool IsEnemyWithinScreen(EnemyBase* e);

	//unsigned calcTime_ = 0;
	//static const unsigned CALC_RAND = 5;	//n回に１回Enemyの計算をする：軽い処理になったら１でもいいかもね

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

	//AimTargetにされているエネミー
	EnemyBase* pAimEnemy = GameManager::GetPlayer()->GetAim()->GetTargetEnemy();
	
	//プレイヤーと比較して近い順のリスト
	vector<EnemyBase*> eList = GameManager::GetEnemyManager()->GetAllEnemy();
	std::sort(eList.begin(), eList.end(), compare);

	//リストの最初の谷津スクリーン内科試す
	if (!eList.empty()) {
		XMFLOAT3 fPos = eList.at(0)->GetPosition();
		Camera::IsPositionWithinVector(fPos);
		Camera::IsPositionWithinScreen(fPos);
	}
	
	//AimTarget以外で一番近いやつ
	if (!eList.empty() && rand() % 10 == 0) {
		if (!pAimEnemy) {
			int random = rand() % 100;
			int count = 0;

			//個数求める
			if (random > 90) count = 3;
			else if (random > 80) count = 2;
			else if (random > 30) count = 1;
			else if (random > 0) count = 0;

			//エネミーの数より多いなら抑制
			if (eList.size() < count) count = (int)eList.size();

			//近い数分フラグをセット
			for (int i = 0; i < count; i++) {
				eList[i]->SetCombatReady(true);
			}
		}
		else {
			int random = rand() % 100;
			int count = 0;

			//個数求める
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
	}
}

bool CombatAI::IsEnemyAttackPermission(EnemyBase* enemy)
{
	if (DifficultyManager::AttackPermission() && enemy->GetCombatReady())
		return true;
	return false;
}

bool CombatAI::IsEnemyMovePermission(EnemyBase* enemy)
{
	//とりあえずのrand	//修正箇所
	if (enemy == GameManager::GetPlayer()->GetAim()->GetTargetEnemy()) return true;

	if (rand() % 3 == 0) return true;
	return false;
}

bool CombatAI::IsEnemyWithinScreen(EnemyBase* e) {
	XMFLOAT3 pos = e->GetPosition();

	XMVECTOR v2 = XMVector3TransformCoord(XMLoadFloat3(&pos), Camera::GetViewMatrix());
	v2 = XMVector3TransformCoord(v2, Camera::GetProjectionMatrix());
	float x = XMVectorGetX(v2);
	float y = XMVectorGetY(v2);

	OutputDebugStringA(std::to_string(XMVectorGetZ(v2)).c_str());
	OutputDebugString("\n"); 

	//反対方向にある場合の条件（みかん
	if (XMVectorGetZ(v2) > 1.0f) return false;

	//画角制限する
	if (x >= 1.0f || y >= 1.0f || x <= -1.0f || y <= -1.0f) return false;
	return true;
}
