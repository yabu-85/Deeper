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

bool DamageCtrl::CalcSword(LineCollider* line, int damage)
{
    std::vector<EnemyBase*> enemyList = pEnemySpawnCtrl_->GetAllEnemy();
	bool hit = false;

    //線分（レイ）と円の衝突判定パクった：おまけに衝突開始地点と終了地点とれる
    for (int i = 0; i < enemyList.size(); i++) {
        std::list<Collider*> col = enemyList.at(i)->GetColliderList();

        //Colliderなかったら次
        if (col.empty()) continue;

		for (int j = 0; j < col.size(); j++) {
			if (col.front()->IsHit(line)) {
				enemyList.at(i)->ApplyDamage(damage);
				hit = true;
				break;
			}
			col.pop_front();
		}
    }
	return hit;
}

bool DamageCtrl::CalcBullet(SphereCollider* sphere, int damage)
{
	std::vector<EnemyBase*> enemyList = pEnemySpawnCtrl_->GetAllEnemy();
	bool hit = false;

	for (int i = 0; i < enemyList.size(); i++) {
		std::list<Collider*> col = enemyList.at(i)->GetColliderList();

		//Colliderなかったら次
		if (col.empty()) continue;

		for (int j = 0; j < col.size(); j++) {
			if (col.front()->IsHit(sphere)) {
				enemyList.at(i)->ApplyDamage(damage);
				hit = true;
				break;
			}
			col.pop_front();
		}
	}
	return hit;
}
