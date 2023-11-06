#include "DropTable.h"
#include "EnemyBase.h"
#include "Player.h"
#include "GameManager.h"
#include "WeaponObjectManager.h"

DropTable::DropTable(EnemyBase* parent)
	:money_(0),parcent_(0), pEnemyBase_(parent)
{
}

void DropTable::DropItem()
{

	if (parcent_ == 0) return;
	if (rand() % 100 < parcent_) {
		GameManager* gm = static_cast<GameManager*>(pEnemyBase_->FindObject("GameManager"));
		WeaponObjectManager* ma = gm->GetWeaponObjectManager();
		ma->AddWeaponObject(WeaponObjectManager::WEAPON_TYPE::WT_SUB1);
	}
	
}
