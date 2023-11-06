#pragma once
#include <vector>

class WeaponObject;
class GameManager;
class WeaponBase;

class WeaponObjectManager
{
public:
	enum WEAPON_TYPE {
		WT_SUB1 = 0,
		WT_SUB2,
		WT_MAX,
	};

private:
	float range_;							//取得できる範囲
	WeaponObject* nearestObject_;			//一番近いWeaponObject
	std::vector<WeaponObject* > objctList_;	//WeaponObjectのリスト
	GameManager* pParent_;					//GameManager

public:
	WeaponObjectManager(GameManager* parent);

	void AddWeaponObject(WEAPON_TYPE type);
	void RemoveWeaponObject(WeaponObject* obj);
	void AllKillWeaponObject();

	//nearestObject_の計算をして、範囲内に一つでもあればtrue
	bool IsInPlayerRange();

	//一番近いWeaponObjectのWeaponBaseのポインタを取得
	WeaponBase* GetNearestWeapon();

};

