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