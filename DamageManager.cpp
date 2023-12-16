#include "DamageManager.h"
#include "EnemyManager.h"
#include "EnemyBase.h"
#include "Player.h"
#include "GameManager.h"
#include "Engine/Input.h"
#include "StateManager.h"
#include <vector>
#include <list>

DamageManager::DamageManager(EnemyManager* p)
	: pEnemyManager_(p)
{
}

DamageManager::~DamageManager()
{
}

void DamageManager::AddCharacter(Character* obj, DamageType _type)
{
	CollisionData chara;
	chara.objct = obj;
	chara.type = _type;
	collisionList_.push_back(chara);
}

void DamageManager::RemoveCharacter(Character* obj)
{
	for (auto it = collisionList_.begin(); it != collisionList_.end();) {
		if (it->objct == obj) {
			it = collisionList_.erase(it);
			break;
		}
		else {
			++it;
		}
	}
}

void DamageManager::AllRemoveCharacter()
{
	collisionList_.clear();
}

bool DamageManager::CalcEnemy(Collider* collider, int damage)
{
    std::vector<EnemyBase*> enemyList = pEnemyManager_->GetAllEnemy();
	bool hit = false;

    for (int i = 0; i < enemyList.size(); i++) {
        std::list<Collider*> col = enemyList.at(i)->GetColliderList();

        // “oê’† || Collider‚È‚©‚Á‚½‚çŽŸ
		if (!enemyList.at(i)->GetStateManager()) continue;
		if (enemyList.at(i)->GetStateManager()->GetName() == "Appear" || col.empty()) continue;

		int size = (int)col.size();
		for (int j = 0; j < size; j++) {
			if (col.front()->IsHit(collider)) {
				enemyList.at(i)->ApplyDamage(damage);
				hit = true;
				break;
			}
			col.pop_front();
		}
    }

	return hit;
}

bool DamageManager::CalcPlyaer(Collider* collider, int damage)
{
	bool hit = false;
	Player* pPlayer = (Player*)GameManager::GetParent()->FindObject("Player");
	std::list<Collider*> col = pPlayer->GetColliderList();

	//Collider‚È‚©‚Á‚½‚çŽŸ
	if (col.empty()) return false;

	int size = (int)col.size();
	for (int j = 0; j < size; j++) {
		if (col.front()->IsHit(collider)) {
			pPlayer->ApplyDamage(damage);
			hit = true;
			break;
		}
		col.pop_front();
	}
	
	return hit;
}
