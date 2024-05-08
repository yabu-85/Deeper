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

	//�݌v���������Ă���
	//�l�������g�̏���
	//Move��Attack

	//�G�l�~�[���ɏ����������H

	//�K�[�h�^�Ɛ���Ă�����A�t�߂̑��G�l�~�[���U�����ɍs���̂�������
	//�K�[�h�^������Ƃ��āA���̃G�l�~�[���U������Ă�������ɓ�����
}

void CombatAI::Initialize()
{
}

void CombatAI::Update() {
	//CALC_RAND�̎��ԂɂȂ�����v�Z����
	if (CALC_RAND > calcTime_++) return;
	calcTime_ = 0;

	//�v���C���[�Ɣ�r���ċ߂����̃��X�g
	vector<EnemyBase*> eList = GameManager::GetEnemyManager()->GetAllEnemy();
	if (eList.empty()) return;
	std::sort(eList.begin(), eList.end(), compare);

	//�����߂�
	int random = rand() % 100;
	int count = 0;
	if (random > 80) count = 2;
	else if (random > 40) count = 1;
	else if (random > 0) count = 0;

	//�G�l�~�[�̐���葽���Ȃ�}��
	if (eList.size() < count) count = (int)eList.size();

	//�߂������t���O���Z�b�g
	for (int i = 0; i < count; i++) {
		eList[i]->SetCombatReady(true);
	}
	
}

bool CombatAI::IsEnemyAttackPermission(EnemyBase* enemy)
{
	if (rand() % 5 == 0) return true;

	return (DifficultyManager::AttackPermission() && enemy->GetCombatReady());
}

bool CombatAI::IsEnemyMovePermission(EnemyBase* enemy)
{
	//�C���ӏ� //�Ƃ肠������rand
	if (enemy == GameManager::GetPlayer()->GetAim()->GetTargetEnemy()) return true;

	if (rand() % 3 == 0) return true;
	return false;
}