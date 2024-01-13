#include "WeaponObjectManager.h"
#include "WeaponObject.h"
#include "Engine/Model.h"
#include "Player.h"
#include "TestWeaponSub.h"
#include "GameManager.h"
#include "PlayerWeapon.h"

//�f�o�b�O�p
namespace {
	Player* pPlayer = nullptr;
}

WeaponObjectManager::WeaponObjectManager()
	: range_(0), nearestObject_(nullptr)
{
	range_ = 3.0f;

	AddWeaponObject(WeaponObjectManager::WEAPON_TYPE::WT_SUB1, XMFLOAT3(50, 0, 50));
	nearestObject_ = objctList_.at(0);
	
	return;
	WeaponBase* weapon = GetNearestWeapon();
	if (weapon) {
		pPlayer->GetPlayerWeapon()->SetWeapon(weapon);
	}
}

WeaponObjectManager::~WeaponObjectManager()
{
}

void WeaponObjectManager::AddWeaponObject(WEAPON_TYPE type, XMFLOAT3 pos)
{
	std::string fileName[(int)WEAPON_TYPE::WT_MAX] = { "Feet", "RedBox" };

	WeaponObject* weapon = Instantiate<WeaponObject>(GameManager::GetParent());

	//�f�o�b�O�p
	if ((int)type == 0) weapon->SetScale(0.2f, 0.2f, 0.2f);

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
	Player* pPlayer = static_cast<Player*>(GameManager::GetParent()->FindObject("Player"));
    XMFLOAT3 plaPos = pPlayer->GetPosition();
	
	int minRangeIndex = -1;
	float minRange = range_;
	
	for (int i = 0; i < objctList_.size(); i++) {
		XMFLOAT3 objPos = objctList_.at(i)->GetPosition();
		objPos = XMFLOAT3(plaPos.x - objPos.x, plaPos.y - objPos.y, plaPos.z - objPos.z);

		float currentSpeed = XMVectorGetX(XMVector3Length(XMLoadFloat3(&objPos)));
		if (minRange > currentSpeed) {
			minRangeIndex = i;
			minRange = currentSpeed;
		}
	}

	if (minRangeIndex >= 0) {
		nearestObject_ = objctList_.at(minRangeIndex);
		return true;
	}

	return false;
}

WeaponBase* WeaponObjectManager::GetNearestWeapon()
{
	if (nearestObject_->GetType() == (int)WEAPON_TYPE::WT_SUB1) {
		RemoveWeaponObject(nearestObject_);
		nearestObject_->KillMe();
		nearestObject_ = nullptr;
		pPlayer = static_cast<Player*>(GameManager::GetParent()->FindObject("Player"));
		return Instantiate<TestWeaponSub>(pPlayer);
	}

	if (nearestObject_->GetType() == (int)WEAPON_TYPE::WT_SUB2) {
		RemoveWeaponObject(nearestObject_);
		nearestObject_->KillMe();
		nearestObject_ = nullptr;
		return nullptr;
	}

	return nullptr;
}
