#include "Character.h"

Character::Character(GameObject* parent) 
	: GameObject(parent), movement_(0,0,0), bodyWeight_(1.0f), bodyRange_(0.5f)
{
}

void Character::ReflectCharacter(Character* pCharacter) {
	//相手の中心方向へのベクトルを求める
	XMFLOAT3 tarPos = pCharacter->GetPosition();
	XMVECTOR direction = XMLoadFloat3(&tarPos) - XMLoadFloat3(&transform_.position_);
	
	//距離が判定内の距離だったら離れさせる
	float range = bodyRange_ + pCharacter->bodyRange_;
	if ( XMVectorGetX(XMVector3Length(direction)) < range ) {
		
		//二つの座標の真ん中を求める
		direction *= 0.5f;
		XMVECTOR sPos = XMLoadFloat3(&transform_.position_) + direction;
		XMVECTOR oPos = XMLoadFloat3(&pCharacter->transform_.position_) - direction;
		
		//押し出すベクトルを求める
		direction = XMVector3Normalize(direction) * range * 0.5f;

		float w = this->bodyWeight_ + pCharacter->bodyWeight_;
		float sWeight = pCharacter->bodyWeight_ / w + 0.001f;
		float oWeight = this->bodyWeight_ / w + 0.001f;

		sWeight = 1.0f;
		oWeight = 1.0f;

		//押し出す
		sPos = sPos - direction * oWeight;
		XMStoreFloat3(&transform_.position_, sPos);
		oPos = oPos + direction * sWeight;
		XMStoreFloat3(&pCharacter->transform_.position_, oPos);

		return;

	}
}