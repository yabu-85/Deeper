#include "Interaction.h"
#include "InteractionUI.h"
#include "../GameManager/GameManager.h"
#include "../Player/Player.h"
#include "../Engine/Transform.h"
#include "../Engine/Global.h"
#include "../Engine/Sprite.h"
#include "../Engine/Camera.h"
#include "../Weapon/WeaponObjectManager.h"
#include <vector>

namespace Interaction {
	int time_ = 0;
	int uiListIndex_ = -1;
	Sprite* interactImage_ = nullptr;
	std::vector<InteractionUI*> uiList_;

}

void Interaction::Initialize() {
	interactImage_ = new Sprite();
	interactImage_->Load("Image/KeyImage/E.jpg");

}

void Interaction::Draw() {
	//WeaponのUIを表示するかどうかを計算
	time_++;
	if (time_ > 5) {
		time_ = 0;
		GameManager::GetWeaponObjectManager()->InteractUIIsInPlayerRange();
	}
	
	uiListIndex_ = -1;
	float minDistance = 99999;
	XMFLOAT3 plaPos = GameManager::GetPlayer()->GetPosition();

	//一番近いUIオブジェクトを計算
	for (int it = 0; it < (int)uiList_.size(); it++) {
		if (!uiList_[it]->GetValid()) continue;
		float dist = DistanceCalculation(plaPos, uiList_[it]->GetParent()->GetPosition());

		if (dist <= minDistance) {
			minDistance = dist;
			uiListIndex_ = it;
		}
	}

	//表示するオブジェクトがあるから計算して表示
	if (uiListIndex_ >= 0) {
		XMFLOAT3 pos = Float3Add(uiList_.at(uiListIndex_)->GetParent()->GetPosition(), uiList_.at(uiListIndex_)->GetOffset());
		XMVECTOR v2 = XMVector3TransformCoord(XMLoadFloat3(&pos), Camera::GetViewMatrix());
		v2 = XMVector3TransformCoord(v2, Camera::GetProjectionMatrix());
		float x = XMVectorGetX(v2);
		float y = XMVectorGetY(v2);

		XMFLOAT3 size = interactImage_->GetTextureSize();
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = (long)size.x;
		rect.bottom = (long)size.y;

		Transform t;
		t.position_ = { x, y, 1.0f };
		t.scale_ = { 1.0f, 1.0f, 1.0f };
		t.Calclation();
		interactImage_->Draw(t, rect, 1.0f, 0);
	}
}

void Interaction::SceneTransitionInitialize()
{
	ResetInteractData();

}

void Interaction::ResetInteractData()
{
	uiList_.clear();
}

void Interaction::AddInteractData(InteractionUI* data)
{
	uiList_.push_back(data);
}

void Interaction::RemoveInteractData(InteractionUI* data)
{
	for (auto it = uiList_.begin(); it != uiList_.end();) {
		if (*it == data) {
			it = uiList_.erase(it);
			break;
		}
		else {
			++it;
		}
	}
}

bool Interaction::IsMinDistance(GameObject* parent)
{
	//エラー処理
	if (uiListIndex_ <= -1 || uiListIndex_ > ((int)uiList_.size() - 1)) return false;

	if (uiList_.at(uiListIndex_)->GetParent() == parent) return true;
	return false;
}
