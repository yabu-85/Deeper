#pragma once
#include <string>

class WeaponBase
{
protected:
	std::string weaponName_;
	float damage_;

public:
	virtual void Initialize() = 0;
};

