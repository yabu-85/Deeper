#pragma once
#include <vector>

class WeaponObject;
class GameObject;

class WeaponObjectManager
{
	GameObject* pParent_;
	std::vector<WeaponObject* > objctList_;

public:
	WeaponObjectManager(GameObject* parent);

	void Update();
	bool IsInPlayerRange();

};

