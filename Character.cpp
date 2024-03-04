#include "Character.h"
#include "GameManager/GameManager.h"
#include <vector>

Character::Character(GameObject* parent, std::string name)
	: GameObject(parent, name), movement_(0,0,0), bodyWeight_(1.0f), bodyRange_(0.5f)
{
}

void Character::ReflectCharacter() {
	std::vector<Character*> cList = GameManager::GetCharacterList();
	float sY = transform_.position_.y;

	for (Character* c : cList) {
		if (c == this) continue;

		float oY = c->transform_.position_.y;

		//相手の中心方向へのベクトルを求める
		XMFLOAT3 tarPos = c->GetPosition();
		XMVECTOR direction = XMLoadFloat3(&tarPos) - XMLoadFloat3(&transform_.position_);

		//距離が判定内の距離だったら離れさせる
		float range = bodyRange_ + c->bodyRange_;
		float vecRange = XMVectorGetX(XMVector3Length(direction));
		if (vecRange < range) {
			//移動量を考慮した座標を設定
			XMVECTOR vTarPos = XMLoadFloat3(&tarPos);
			direction = vTarPos - XMLoadFloat3(&transform_.position_);
			vecRange = XMVectorGetX(XMVector3Length(direction));

			//二つの座標の真ん中を求める
			direction *= 0.5f;
			XMVECTOR sPos = XMLoadFloat3(&transform_.position_) + direction;
			XMVECTOR oPos = XMLoadFloat3(&c->transform_.position_) - direction;

			//押し出しベクトルを求める
			float w = this->bodyWeight_ + c->bodyWeight_;
			float sWeight = c->bodyWeight_ / w + 0.001f;
			float oWeight = this->bodyWeight_ / w + 0.001f;
			direction = XMVector3Normalize(direction) * range * 0.5f;
			XMVECTOR push = direction * (range - vecRange);

			//押し出す
			sPos = sPos - direction - (push * sWeight);
			XMStoreFloat3(&transform_.position_, sPos);
			oPos = oPos + direction + (push * oWeight);
			XMStoreFloat3(&c->transform_.position_, oPos);
			
			transform_.position_.y = oY;
		}
	}
}