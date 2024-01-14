#pragma once
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;
class WeaponObject;
class WeaponBase;

class WeaponObjectManager
{
	float range_;							//取得できる範囲
	WeaponObject* nearestObject_;			//一番近いWeaponObject
	std::vector<WeaponObject* > objctList_;	//WeaponObjectのリスト
	
public:
	enum class WEAPON_TYPE {
		WT_SUB1 = 0,
		WT_SUB2,
		WT_MAX,
	};

	WeaponObjectManager();
	~WeaponObjectManager();
	void SceneTransitionInitialize();
	void AddWeaponObject(WEAPON_TYPE type, XMFLOAT3 pos);
	void RemoveWeaponObject(WeaponObject* obj);
	void AllKillWeaponObject();

	//範囲内に一つでもあればtrue / nearestObjectの更新も担ってる
	bool IsInPlayerRange();

	//一番近いWeaponObjectのWeaponBaseのポインタを取得
	WeaponBase* GetNearestWeapon();

};

