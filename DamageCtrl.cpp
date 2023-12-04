#include "DamageCtrl.h"
#include "EnemySpawnCtrl.h"
#include "EnemyBase.h"
#include <vector>
#include <list>
#include "Engine/LineCollider.h"
#include "Player.h"
#include "GameManager.h"

#include "Engine/Input.h"

DamageCtrl::DamageCtrl(EnemySpawnCtrl* p)
	: pEnemySpawnCtrl_(p)
{
}

DamageCtrl::~DamageCtrl()
{
}

void DamageCtrl::Update()
{
	//��������Ȃ�
	return;

	for (CollisionData chara : collisionList_) {
		//������AttackCollider�������list�ɓ���Ď���
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

				// target�� chara��type���Ⴄ�Ȃ�tCollider�ɒǉ�
				if (it != calcList.end() && it->type != chara.type) {
					std::list<Collider*> col = it->objct->GetColliderList();

					// �A������֐�
					tCollider.splice(tCollider.end(), col);
				}

				//���X�g����폜:�����A�����鏈���̑O�ɂ���it�����̃C�e���[�^�ɂȂ邩�璍��
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

void DamageCtrl::AddCharacter(Character* obj, DamageType _type)
{
	CollisionData chara;
	chara.objct = obj;
	chara.type = _type;
	collisionList_.push_back(chara);
}

void DamageCtrl::RemoveCharacter(Character* obj)
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

void DamageCtrl::AllRemoveCharacter()
{
	collisionList_.clear();
}

bool DamageCtrl::CalcSword(LineCollider* line, int damage)
{
    std::vector<EnemyBase*> enemyList = pEnemySpawnCtrl_->GetAllEnemy();
	bool hit = false;

    for (int i = 0; i < enemyList.size(); i++) {
        std::list<Collider*> col = enemyList.at(i)->GetColliderList();

        // Collider�Ȃ������玟
        if (col.empty()) continue;

		int size = (int)col.size();
		for (int j = 0; j < size; j++) {
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
		
		//Collider�Ȃ������玟
		if (col.empty()) continue;

		int size = (int)col.size();
		for (int j = 0; j < size; j++) {
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

bool DamageCtrl::CalcPlyaer(SphereCollider* sphere, int damage)
{
	bool hit = false;
	Player* pPlayer = (Player*)GameManager::GetParent()->FindObject("Player");
	std::list<Collider*> col = pPlayer->GetColliderList();

	//Collider�Ȃ������玟
	if (col.empty()) return false;

	int size = (int)col.size();
	for (int j = 0; j < size; j++) {
		if (col.front()->IsHit(sphere)) {
			pPlayer->ApplyDamage(damage);
			hit = true;
			break;
		}
		col.pop_front();
	}
	
	return hit;
}
