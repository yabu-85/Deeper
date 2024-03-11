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
	
	void CaclNearestObject();

public:
	enum class WEAPON_TYPE {
		WT_STONE = 0,
		WT_THROW,
		WT_MELEE,
		WT_MAX,
	};

	WeaponObjectManager();
	~WeaponObjectManager();
	void SceneTransitionInitialize();

	//全てのWeaponObjectをKill
	void AllKillWeaponObject();
	
	//Listに追加
	void AddWeaponObject(WEAPON_TYPE type, XMFLOAT3 pos);
	//Listから削除
	void RemoveWeaponObject(WeaponObject* obj);

	//範囲内にWeaponObjectがあるかどうか
	bool IsInPlayerRange();

	//一番近いWeaponObjectを計算してそのWeaponを取得
	WeaponBase* GetNearestWeapon();

	//Weapon全てUIを表示するかどうか
	void InteractUIIsInPlayerRange();


};

