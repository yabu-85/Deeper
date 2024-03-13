#pragma once
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;
class WeaponObject;
class WeaponBase;

class WeaponObjectManager
{
	float range_;							//�擾�ł���͈�
	WeaponObject* nearestObject_;			//��ԋ߂�WeaponObject
	std::vector<WeaponObject* > objctList_;	//WeaponObject�̃��X�g
	
	void CaclNearestObject();

public:
	enum WEAPON_TYPE {
		WT_STONE = 0,
		WT_THROW,
		WT_MELEE,
		WT_MAX,
	};

	WeaponObjectManager();
	~WeaponObjectManager();
	void SceneTransitionInitialize();

	//�S�Ă�WeaponObject��Kill
	void AllKillWeaponObject();
	
	//List�ɒǉ�
	void AddWeaponObject(WEAPON_TYPE type, XMFLOAT3 pos);
	//List����폜
	void RemoveWeaponObject(WeaponObject* obj);

	//�͈͓���WeaponObject�����邩�ǂ���
	bool IsInPlayerRange();

	//��ԋ߂�WeaponObject���v�Z���Ă���Weapon���擾
	WeaponBase* GetNearestWeapon();

	//Weapon�S��UI��\�����邩�ǂ���
	void InteractUIIsInPlayerRange();


};

