#include "CombatAI.h"
#include "GameManager.h"
#include "DifficultyManager.h"
#include "Player/Player.h"
#include "Player/Aim.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/EnemyManager.h"
#include "State/StateManager.h"
#include <vector>

namespace CombatAI {
	unsigned calcTime_ = 0;
	static const unsigned CALC_RAND = 5;	//n回に１回Enemyの計算をする：軽い処理になったら１でもいいかもね
	
	void Initialize() {

	}

	void Update(){
		calcTime_++;
		if (calcTime_ % CALC_RAND != 0) return;
		
		EnemyBase* pAimEnemy = GameManager::GetPlayer()->GetAim()->GetTargetEnemy();
		float minCombatDist = 99999.9f;

		for (auto e : GameManager::GetEnemyManager()->GetAllEnemy()) {
			//CombatStateでもない・すでに攻撃Stateなら計算しない
			if (e->GetStateManager()->GetName() != "Combat" || e->GetCombatStateManager()->GetName() == "Attack") continue;
			



		
		}
	}

	bool IsEnemyAttackPermission(EnemyBase* enemy)
	{
		//いったんこれで
		return DifficultyManager::AttackPermission();


		//危険度やばめだから行動しちゃダメ
		if (!DifficultyManager::AttackPermission()) return false;

		XMFLOAT3 plaPos = GameManager::GetPlayer()->GetPosition();
		XMFLOAT3 enePos = enemy->GetPosition();
		XMFLOAT3 vec = { plaPos.x - enePos.x, 0.0f, plaPos.z - enePos.z };

		//距離計算して範囲内なら行動可能
		float dist = sqrtf(vec.x * vec.x + vec.z * vec.z);
		if (dist <= enemy->GetCombatDistance()) return true;

		//範囲外だと敵全員動かんかもしれん
		//一番近い敵やったら行動してええみたいな処理が必要になる

		return false;
	}

}