#include "WeaponObjectManager.h"
#include "WeaponObject.h"
#include "TestWeaponSub.h"
#include "StoneArmWeapon.h"
#include "../Engine/Model.h"
#include "../Player/Player.h"
#include "../GameManager/GameManager.h"
#include "../UI/InteractionUI.h"

WeaponObjectManager::WeaponObjectManager()
	: range_(0), nearestObject_(nullptr)
{
	range_ = 1.0f;
}

WeaponObjectManager::~WeaponObjectManager()
{
}

void WeaponObjectManager::SceneTransitionInitialize()
{
	objctList_.clear();
	nearestObject_ = nullptr;

}

void WeaponObjectManager::AddWeaponObject(WEAPON_TYPE type, XMFLOAT3 pos)
{
	std::string fileName[(int)WEAPON_TYPE::WT_MAX] = { "StoneArmObj", "RedBoxObj", "BlueBox"};

	WeaponObject* weapon = InstantiateFront<WeaponObject>(GameManager::GetStage());

	weapon->SetPosition(pos);
	weapon->LoadModel(fileName[(int)type]);
	weapon->SetType(int(type));
	objctList_.push_back(weapon);
}

void WeaponObjectManager::RemoveWeaponObject(WeaponObject* obj)
{
	for (auto it = objctList_.begin(); it != objctList_.end();) {
		if (*it == obj) {
			it = objctList_.erase(it);
			return;
		}
		else {
			it++;
		}
	}
}

void WeaponObjectManager::AllKillWeaponObject()
{
	for (WeaponObject* obj : objctList_) {
		obj->KillMe();
	}
	objctList_.clear();
	nearestObject_ = nullptr;
}

bool WeaponObjectManager::IsInPlayerRange()
{
	XMFLOAT3 plaPos = GameManager::GetPlayer()->GetPosition();

	for (int i = 0; i < objctList_.size(); i++) {
		XMFLOAT3 objPos = objctList_.at(i)->GetPosition();
		objPos = XMFLOAT3(plaPos.x - objPos.x, plaPos.y - objPos.y, plaPos.z - objPos.z);

		float range = XMVectorGetX(XMVector3Length(XMLoadFloat3(&objPos)));
		if (range <= range_) {
			return true;
		}
	}

	return false;
}

WeaponObject* WeaponObjectManager::GetNearestWeapon()
{
	CaclNearestObject();

	return nearestObject_;
}

WeaponBase* WeaponObjectManager::PlayerWeaponSet()
{
	CaclNearestObject();

	if (nearestObject_ == nullptr) {
		OutputDebugString("nearestObject->Is NullPtr Deth");
		return nullptr;
	}

	WeaponBase* weapon = nullptr;
	if (nearestObject_->GetType() == (int)WEAPON_TYPE::WT_STONE) weapon = Instantiate<StoneArmWeapon>(GameManager::GetPlayer());
	else if (nearestObject_->GetType() == (int)WEAPON_TYPE::WT_THROW) weapon = Instantiate<TestWeaponSub>(GameManager::GetPlayer());
	else if (nearestObject_->GetType() == (int)WEAPON_TYPE::WT_MELEE) weapon = Instantiate<TestWeaponSub>(GameManager::GetPlayer());
	
	RemoveWeaponObject(nearestObject_);
	nearestObject_->KillMe();
	nearestObject_ = nullptr;

	return weapon;
}

void WeaponObjectManager::CaclNearestObject()
{
	Player* pPlayer = GameManager::GetPlayer();
	XMFLOAT3 plaPos = pPlayer->GetPosition();

	int minRangeIndex = -1;
	float minRange = range_;

	for (int i = 0; i < objctList_.size(); i++) {
		XMFLOAT3 objPos = objctList_.at(i)->GetPosition();
		objPos = XMFLOAT3(plaPos.x - objPos.x, plaPos.y - objPos.y, plaPos.z - objPos.z);

		float range = XMVectorGetX(XMVector3Length(XMLoadFloat3(&objPos)));
		if (minRange > range) {
			minRangeIndex = i;
			minRange = range;
		}
	}

	nearestObject_ = nullptr;
	if (minRangeIndex >= 0) {
		nearestObject_ = objctList_.at(minRangeIndex);
	}

}

void WeaponObjectManager::InteractUIIsInPlayerRange()
{
	XMFLOAT3 plaPos = GameManager::GetPlayer()->GetPosition();

	for (int i = 0; i < objctList_.size(); i++) {
		XMFLOAT3 objPos = objctList_.at(i)->GetPosition();
		objPos = XMFLOAT3(plaPos.x - objPos.x, plaPos.y - objPos.y, plaPos.z - objPos.z);

		float range = XMVectorGetX(XMVector3Length(XMLoadFloat3(&objPos)));
		if (range <= range_) objctList_.at(i)->GetInteractionUI()->SetValid(true);
		else objctList_.at(i)->GetInteractionUI()->SetValid(false);
	}
}
