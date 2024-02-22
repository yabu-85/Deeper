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
	static const unsigned CALC_RAND = 5;	//n��ɂP��Enemy�̌v�Z������F�y�������ɂȂ�����P�ł�����������
	
	void Initialize() {

	}

	void Update(){
		calcTime_++;
		if (calcTime_ % CALC_RAND != 0) return;
		
		//������Enemy���Ƃ̂��
		//IsEnemy�Ȃ񂽂�Ń|�C���^�̃G�l�~�[�͍s�������Ă�������bool�̂�
		//�v�Z�������̂�n��

		EnemyBase* pAimEnemy = GameManager::GetPlayer()->GetAim()->GetTargetEnemy();
		float minCombatDist = 99999.9f;

		for (auto e : GameManager::GetEnemyManager()->GetAllEnemy()) {
			//CombatState�ł��Ȃ��E���łɍU��State�Ȃ�v�Z���Ȃ�
			if (e->GetStateManager()->GetName() != "Combat" || e->GetCombatStateManager()->GetName() == "Attack") continue;
			



		
		}
	}

	bool IsEnemyAttackPermission(EnemyBase* enemy)
	{
		//�������񂱂��
		return DifficultyManager::AttackPermission();


		//�댯�x��΂߂�����s��������_��
		if (!DifficultyManager::AttackPermission()) return false;

		XMFLOAT3 plaPos = GameManager::GetPlayer()->GetPosition();
		XMFLOAT3 enePos = enemy->GetPosition();
		XMFLOAT3 vec = { plaPos.x - enePos.x, 0.0f, plaPos.z - enePos.z };

		//�����v�Z���Ĕ͈͓��Ȃ�s���\
		float dist = sqrtf(vec.x * vec.x + vec.z * vec.z);
		if (dist <= enemy->GetCombatDistance()) return true;

		//�͈͊O���ƓG�S�������񂩂������
		//��ԋ߂��G�������s�����Ă����݂����ȏ������K�v�ɂȂ�

		return false;
	}

}