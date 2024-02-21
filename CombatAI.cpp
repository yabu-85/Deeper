#include "CombatAI.h"
#include "GameManager.h"
#include "DifficultyManager.h"

#include "Player/Player.h"
#include "Enemy/EnemyBase.h"

namespace CombatAI {
	float minDistance_ = 0.0f;
	
	void Initialize() {

	}

	void Update(){

	}

	bool IsEnemyAttackPermission(EnemyBase* enemy)
	{
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