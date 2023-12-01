#include "DamageCtrl.h"
#include "EnemySpawnCtrl.h"
#include "EnemyBase.h"
#include <vector>
#include <list>
#include "Engine/LineCollider.h"
#include "Player.h"
#include "GameManager.h"

DamageCtrl::DamageCtrl(EnemySpawnCtrl* p)
	: pEnemySpawnCtrl_(p)
{
}

DamageCtrl::~DamageCtrl()
{
}

void DamageCtrl::Update()
{
	for (Character chara : calcCharacters_) {
		std::list<Collider*> sCollider = chara.objct->GetColliderList();
		if (sCollider.empty()) continue;

		// ©•ªˆÈŠO‚ÌColliderEˆá‚¤DamageType‚ÌCollider ‚ğæ“¾
		std::list<Collider*> tCollider;
		for (Character c : calcCharacters_) {
			if (c.objct == chara.objct || c.type == chara.type) continue;
			std::list<Collider*> col = c.objct->GetColliderList();
			tCollider.splice(tCollider.end(), col);
		}
		if (tCollider.empty()) continue;

		for (Collider* sc : sCollider) {
			for (Collider* tc : tCollider) {
				if (sc->IsHit(tc)) {
					//‚±‚±‚Å‚ ‚½‚Á‚½‚æŠÖ”‚ğŒÄ‚Ô
					chara.objct->OnCollision(tc->GetGameObject());
					sCollider.clear();
					break;
				}
			}
			if (sCollider.empty()) break;
		}
	}
}

void DamageCtrl::AddCharacter(GameObject* obj, DamageType _type)
{
	Character chara;
	chara.objct = obj;
	chara.type = _type;
	calcCharacters_.push_back(chara);
}

void DamageCtrl::RemoveCharacter(GameObject* obj)
{

}

bool DamageCtrl::CalcSword(LineCollider* line, int damage)
{
    std::vector<EnemyBase*> enemyList = pEnemySpawnCtrl_->GetAllEnemy();
	bool hit = false;

    for (int i = 0; i < enemyList.size(); i++) {
        std::list<Collider*> col = enemyList.at(i)->GetColliderList();

        //Collider‚È‚©‚Á‚½‚çŸ
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
		
		//Collider‚È‚©‚Á‚½‚çŸ
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

	//Collider‚È‚©‚Á‚½‚çŸ
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
