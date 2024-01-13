#include "Character.h"

Character::Character(GameObject* parent) 
	: GameObject(parent), movement_(XMVectorZero())
{
}
