#include "CombatAI.h"
#include "GameManager.h"
#include "DifficultyManager.h"
#include "../Player/Player.h"
#include "../Player/Aim.h"
#include "../Enemy/EnemyBase.h"
#include "../Enemy/EnemyManager.h"
#include "../State/StateManager.h"
#include <vector>
#include <queue>

namespace CombatAI {
	unsigned calcTime_ = 0;
	static const unsigned CALC_RAND = 5;	//n��ɂP��Enemy�̌v�Z������F�y�������ɂȂ�����P�ł�����������
	
	void Initialize() {

	}

	void Update(){
		calcTime_++;
		if (calcTime_ % CALC_RAND != 0) return;
		
		//�K�[�h�^�Ɛ���Ă�����A�t�߂̑��G�l�~�[���U�����ɍs���̂�������
		//�K�[�h�^������Ƃ��āA���̃G�l�~�[���U������Ă�������ɓ�����
		
		EnemyBase* pAimEnemy = GameManager::GetPlayer()->GetAim()->GetTargetEnemy();
		std::vector<EnemyBase*> eList = GameManager::GetEnemyManager()->GetAllEnemy();
		XMFLOAT3 plaPos = GameManager::GetPlayer()->GetPosition();

		struct EnemyData {
			int index;
			float distance;
			EnemyData(int i, float dist) : index(i), distance(dist) {}
		}; 
		struct CompareEnemyDatas {
			bool operator()(const EnemyData& a, const EnemyData& b) {
				return a.distance > b.distance;
			}
		};
		std::priority_queue<EnemyData, std::vector<EnemyData>, CompareEnemyDatas> dataList;
		
		for (int i = 0; i < eList.size();i++) {
			//CombatState�ł��Ȃ��E���łɍU��State�Ȃ�v�Z���Ȃ�
			eList[i]->SetCombatReady(false);
			if (eList[i]->GetStateManager()->GetName() != "Combat" || eList[i]->GetCombatStateManager()->GetName() == "Attack") continue;
			
			//AimTarget��Enemy�͗D��I��
			if (pAimEnemy == eList[i] && rand() % 5 == 0) {
				eList[i]->SetCombatReady(true);
				break;
			}
			//AimTarget����Ȃ����
			else {
				XMFLOAT3 enePos = eList[i]->GetPosition();
				XMFLOAT3 vec = { plaPos.x - enePos.x, 0.0f, plaPos.z - enePos.z };
				float dist = sqrtf(vec.x * vec.x + vec.z * vec.z);
				dataList.push({ i, dist });
			}
		}

		//AimTarget�ȊO�ň�ԋ߂����
		if (!dataList.empty() && rand() % 10 == 0) {
			if (!pAimEnemy) {
				int random = rand() % 100;
				int count = 0;

				//�����߂�
				if (random > 90) count = 3;
				else if (random > 80) count = 2;
				else if (random > 30) count = 1;
				else if (random > 0) count = 0;

				//�G�l�~�[�̐���葽���Ȃ�}��
				if (dataList.size() < count) count = (int)dataList.size();

				//�߂������t���O���Z�b�g
				for (int i = 0; i < count; i++) {
					eList[dataList.top().index]->SetCombatReady(true);
					dataList.pop();
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
				if (dataList.size() < count) count = (int)dataList.size();

				//�߂������t���O���Z�b�g
				for (int i = 0; i < count; i++) {
					eList[dataList.top().index]->SetCombatReady(true);
					dataList.pop();
				}
			}
		}
	}

	bool IsEnemyAttackPermission(EnemyBase* enemy)
	{
		if (DifficultyManager::AttackPermission() && enemy->GetCombatReady())
			return true;
		return false;
	}

	bool IsEnemyMovePermission(EnemyBase* enemy)
	{
		//�Ƃ肠������rand	//�C���ӏ�
		if (enemy == GameManager::GetPlayer()->GetAim()->GetTargetEnemy()) return true;

		if (rand() % 3 == 0) return true;
		return false;
	}

}