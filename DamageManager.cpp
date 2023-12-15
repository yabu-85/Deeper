#include "DamageManager.h"
#include "EnemyManager.h"
#include "EnemyBase.h"
#include <vector>
#include <list>
#include "Engine/LineCollider.h"
#include "Player.h"
#include "GameManager.h"
#include "Engine/Input.h"

DamageManager::DamageManager(EnemyManager* p)
	: pEnemyManager_(p)
{
}

DamageManager::~DamageManager()
{
}

void DamageManager::Update()
{
	//いったんなし
	return;

	for (CollisionData chara : collisionList_) {
		//自分のAttackColliderがあればlistに入れて次へ
		std::list<Collider*> sCollider = chara.objct->GetAttackColliderList();
		if (sCollider.empty()) continue;

		std::vector<CollisionData> calcList = collisionList_;
		std::list<Collider*> tCollider;
		Character* target = calcList.front().objct;
		if (target == chara.objct) {
			continue;
		}

		for (auto it = calcList.begin(); it != calcList.end();) {
			if (it->objct == target) {

				// targetが charaとtypeが違うならtColliderに追加
				if (it != calcList.end() && it->type != chara.type) {
					std::list<Collider*> col = it->objct->GetColliderList();

					// 連結する関数
					tCollider.splice(tCollider.end(), col);
				}

				//リストから削除:ここ連結する処理の前にやるとitが次のイテレータになるから注意
				it = calcList.erase(it);

			}
			else {
				++it;
			}
		}
		
		for (Collider* sc : sCollider) {
			if (tCollider.empty()) continue;

			for (Collider* tc : tCollider) {
				if (sc->IsHit(tc)) {
					target->ApplyDamage(1);
					sCollider.clear();
					break;
				}
			}
			if (sCollider.empty()) break;
		}
	}
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

        // Colliderなかったら次
        if (col.empty()) continue;

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

	//Colliderなかったら次
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
