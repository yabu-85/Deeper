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
	static const unsigned CALC_RAND = 5;	//n‰ñ‚É‚P‰ñEnemy‚ÌŒvŽZ‚ð‚·‚éFŒy‚¢ˆ—‚É‚È‚Á‚½‚ç‚P‚Å‚à‚¢‚¢‚©‚à‚Ë
	
	void Initialize() {

	}

	void Update(){
		calcTime_++;
		if (calcTime_ % CALC_RAND != 0) return;
		
		EnemyBase* pAimEnemy = GameManager::GetPlayer()->GetAim()->GetTargetEnemy();
		std::vector<EnemyBase*> eList = GameManager::GetEnemyManager()->GetAllEnemy();
		float minCombatDist = 99999.9f;
		int minCombatIndex = -1;
		XMFLOAT3 plaPos = GameManager::GetPlayer()->GetPosition();
		
		for (int i = 0; i < eList.size();i++) {
			//CombatState‚Å‚à‚È‚¢E‚·‚Å‚ÉUŒ‚State‚È‚çŒvŽZ‚µ‚È‚¢
			eList[i]->SetCombatReady(false);
			if (eList[i]->GetStateManager()->GetName() != "Combat" || eList[i]->GetCombatStateManager()->GetName() == "Attack") continue;
			
			//AimTarget‚ÌEnemy‚Í—Dæ“I‚É
			if (pAimEnemy == eList[i] && rand() % 2 == 0) {
				eList[i]->SetCombatReady(true);
				break;
			}
			//AimTarget‚¶‚á‚È‚¢‚â‚Â
			else {
				XMFLOAT3 enePos = eList[i]->GetPosition();
				XMFLOAT3 vec = { plaPos.x - enePos.x, 0.0f, plaPos.z - enePos.z };
				float dist = sqrtf(vec.x * vec.x + vec.z * vec.z);
				if (dist <= minCombatDist) minCombatIndex = i;
			}
		}

		//AimTargetˆÈŠO‚Åˆê”Ô‹ß‚¢‚â‚Â
		if (minCombatIndex >= 0 && rand() % 5 == 0) {
			eList[minCombatIndex]->SetCombatReady(true);
		}

	}

	bool IsEnemyAttackPermission(EnemyBase* enemy)
	{
		if (DifficultyManager::AttackPermission() && enemy->GetCombatReady()) return true;
		return false;
	}

}