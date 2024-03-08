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
		//�G���[
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
	//static const unsigned CALC_RAND = 5;	//n��ɂP��Enemy�̌v�Z������F�y�������ɂȂ�����P�ł�����������

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

	//AimTarget�ɂ���Ă���G�l�~�[
	EnemyBase* pAimEnemy = GameManager::GetPlayer()->GetAim()->GetTargetEnemy();
	
	//�v���C���[�Ɣ�r���ċ߂����̃��X�g
	vector<EnemyBase*> eList = GameManager::GetEnemyManager()->GetAllEnemy();
	std::sort(eList.begin(), eList.end(), compare);

	//���X�g�̍ŏ��̒J�ÃX�N���[�����Ȏ���
	if (!eList.empty()) {
		XMFLOAT3 fPos = eList.at(0)->GetPosition();
		Camera::IsPositionWithinVector(fPos);
		Camera::IsPositionWithinScreen(fPos);
	}
	
	//AimTarget�ȊO�ň�ԋ߂����
	if (!eList.empty() && rand() % 10 == 0) {
		if (!pAimEnemy) {
			int random = rand() % 100;
			int count = 0;

			//�����߂�
			if (random > 90) count = 3;
			else if (random > 80) count = 2;
			else if (random > 30) count = 1;
			else if (random > 0) count = 0;

			//�G�l�~�[�̐���葽���Ȃ�}��
			if (eList.size() < count) count = (int)eList.size();

			//�߂������t���O���Z�b�g
			for (int i = 0; i < count; i++) {
				eList[i]->SetCombatReady(true);
			}
		}
		else {
			int random = rand() % 100;
			int count = 0;

			//�����߂�
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
	//�Ƃ肠������rand	//�C���ӏ�
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

	//���Ε����ɂ���ꍇ�̏����i�݂���
	if (XMVectorGetZ(v2) > 1.0f) return false;

	//��p��������
	if (x >= 1.0f || y >= 1.0f || x <= -1.0f || y <= -1.0f) return false;
	return true;
}
