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

//デバッグ用
#include "../Engine/Input.h"

namespace Interaction {
	//計算用変数
	int time_ = 0;

	int uiListIndex_ = -1;					//一番近いUI付きオブジェクトのインデックス
	std::vector<InteractionUI*> uiList_;	//UI付きオブジェクトのリスト
	float parcent_ = 0.0f;					//Interactionのパーセント０～１

	//画像データ色なし・色あり
	Sprite* interactImage_ = nullptr;
	Sprite* interactImageSelect_ = nullptr;
}

void Interaction::Initialize() {
	interactImage_ = new Sprite();
	interactImage_->Load("Image/KeyImage/E.jpg");
	interactImageSelect_ = new Sprite();
	interactImageSelect_->Load("Image/KeyImage/EYellow.png");
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
		pos = Camera::CalcScreenPosition(pos);

		XMFLOAT3 size = interactImage_->GetTextureSize();
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = (long)size.x;
		rect.bottom = (long)size.y;

		Transform t;
		t.position_ = { pos.x, pos.y, 1.0f };
		t.scale_ = { 1.0f, 1.0f, 1.0f };
		t.Calclation();
		interactImage_->Draw(t, rect, 1.0f);

		float ysub = ((float)rect.bottom / (float)Direct3D::screenHeight_);
		t.position_.y -= ysub * (1.0f - parcent_);
		t.Calclation();
		rect.top = (LONG)((float)rect.bottom * (1.0f - parcent_));
		rect.bottom = (LONG)((float)rect.bottom * parcent_);
		interactImageSelect_->Draw(t, rect, 1.0f);
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

void Interaction::SetParcent(float f)
{
	parcent_ = f;

}