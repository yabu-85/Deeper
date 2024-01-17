#include "Character.h"

Character::Character(GameObject* parent) 
	: GameObject(parent), movement_(XMVectorZero()), bodyRadius_(1.0f), bodyWeight_(1.0f)
{
}

void Character::ReflectCharacter(Character* pCharacter) {
	//相手の中心方向へのベクトルを求める
	XMFLOAT3 tarPos = pCharacter->GetPosition();
	XMVECTOR direction = XMLoadFloat3(&tarPos) - XMLoadFloat3(&transform_.position_);
	
	//半径より大きければ終わり
	float leng = XMVectorGetX(XMVector3Length(direction));
	//if (leng > this->bodyRadius_ + pCharacter->bodyRadius_) return;
	direction = XMVector3Normalize(direction);

	float w = this->bodyWeight_ + pCharacter->bodyWeight_;
	float weight = pCharacter->bodyWeight_ / w + 0.001f;
	XMVECTOR npos = XMLoadFloat3(&transform_.position_) - (direction * weight * 0.1f);
	XMStoreFloat3(&transform_.position_, npos);

	weight = this->bodyWeight_ / w + 0.001f;
	npos = XMLoadFloat3(&tarPos) + (direction * weight * 0.1f);
	XMStoreFloat3(&pCharacter->transform_.position_, npos);

}