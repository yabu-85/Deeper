#include "Character.h"

Character::Character(GameObject* parent) 
	: GameObject(parent), movement_(XMVectorZero()), bodyWeight_(1.0f)
{
}

void Character::ReflectCharacter(Character* pCharacter) {
	//����̒��S�����ւ̃x�N�g�������߂�
	XMFLOAT3 tarPos = pCharacter->GetPosition();
	XMVECTOR direction = XMLoadFloat3(&tarPos) - XMLoadFloat3(&transform_.position_);
	
	//���a���傫����ΏI���
	float leng = XMVectorGetX(XMVector3Length(direction));
	direction = XMVector3Normalize(direction);

	float w = this->bodyWeight_ + pCharacter->bodyWeight_;
	float weight = pCharacter->bodyWeight_ / w + 0.001f;
	XMVECTOR npos = XMLoadFloat3(&transform_.position_) - (direction * weight * 0.1f);
	XMStoreFloat3(&transform_.position_, npos);

	weight = this->bodyWeight_ / w + 0.001f;
	npos = XMLoadFloat3(&tarPos) + (direction * weight * 0.1f);
	XMStoreFloat3(&pCharacter->transform_.position_, npos);

}