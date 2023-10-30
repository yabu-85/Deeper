#pragma once
#include <string>

class WeaponBase
{
protected:
	std::string name_;
	float damage_;

public:
	virtual void Initialize() = 0;
};

