#include "WeaponObjectManager.h"
#include "WeaponObject.h"
#include "Engine/Model.h"

WeaponObjectManager::WeaponObjectManager(GameObject* parent)
	: pParent_(parent)
{
	objctList_.push_back(Instantiate<WeaponObject>(pParent_));
}

void WeaponObjectManager::Update()
{
}

bool WeaponObjectManager::IsInPlayerRange()
{
	return false;
}
