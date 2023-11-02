#include "DamageCtrl.h"
#include "EnemySpawnCtrl.h"
#include "EnemyBase.h"
#include <vector>
#include <list>
#include "Engine/LineCollider.h"

DamageCtrl::DamageCtrl(EnemySpawnCtrl* p)
	: pEnemySpawnCtrl_(p)
{
}

DamageCtrl::~DamageCtrl()
{
}

void DamageCtrl::ApplyDamage(DAMAGE_TYPE type, int d)
{
	std::vector<EnemyBase*> enemyList = pEnemySpawnCtrl_->GetAllEnemy();
	if (enemyList.empty()) return;

	if (type == DAMAGE_TYPE::ALL) {
		for (EnemyBase* e : enemyList) {
			e->ApplyDamage(d);
		}
	}
	else if (type == DAMAGE_TYPE::RAND) {
		int randomIndex = rand() % (int)enemyList.size();
		enemyList.at(randomIndex)->ApplyDamage(d);
	}

}

int DamageCtrl::CalcSword(LineCollider* line)
{
    std::vector<EnemyBase*> enemyList = pEnemySpawnCtrl_->GetAllEnemy();

    //�����i���C�j�Ɖ~�̏Փ˔���p�N�����F���܂��ɏՓˊJ�n�n�_�ƏI���n�_�Ƃ��
    for (int i = 0; i < enemyList.size(); i++) {
        std::list<Collider*> col = enemyList.at(i)->GetColliderList();

        //Collider�Ȃ������玟
        if (col.empty()) continue;

		for (int j = 0; j < col.size(); j++) {
			if (col.front()->IsHit(line)) {
				enemyList.at(i)->ApplyDamage(2);
				break;
			}
			col.pop_front();
		}

    }
    
	return -1;
}
