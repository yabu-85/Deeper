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

//�f�o�b�O�p
#include "../Engine/Input.h"

namespace Interaction {
	//�v�Z�p�ϐ�
	int time_ = 0;

	int uiListIndex_ = -1;					//��ԋ߂�UI�t���I�u�W�F�N�g�̃C���f�b�N�X
	std::vector<InteractionUI*> uiList_;	//UI�t���I�u�W�F�N�g�̃��X�g
	float parcent_ = 0.0f;					//Interaction�̃p�[�Z���g�O�`�P

	//�摜�f�[�^�F�Ȃ��E�F����
	Sprite* interactImage_ = nullptr;
	Sprite* interactImageSelect_ = nullptr;
	Sprite* interactLongImage_ = nullptr;

}

void Interaction::Initialize() {
	interactImage_ = new Sprite();
	interactImage_->Load("Image/KeyImage/E.jpg");
	interactImageSelect_ = new Sprite();
	interactImageSelect_->Load("Image/KeyImage/EYellow.png");
	interactLongImage_ = new Sprite();
	interactLongImage_->Load("Image/LongPress.png");

}

void Interaction::Draw() {
	//Weapon��UI��\�����邩�ǂ������v�Z
	time_++;
	if (time_ > 5) {
		time_ = 0;
		GameManager::GetWeaponObjectManager()->InteractUIIsInPlayerRange();
	}
	
	uiListIndex_ = -1;
	float minDistance = 99999;
	XMFLOAT3 plaPos = GameManager::GetPlayer()->GetPosition();

	//��ԋ߂�UI�I�u�W�F�N�g���v�Z
	for (int it = 0; it < (int)uiList_.size(); it++) {
		if (!uiList_[it]->GetValid()) continue;
		float dist = DistanceCalculation(plaPos, uiList_[it]->GetParent()->GetPosition());

		if (dist <= minDistance) {
			minDistance = dist;
			uiListIndex_ = it;
		}
	}

	//�\������I�u�W�F�N�g�����邩��v�Z���ĕ\��
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

		if ((uiList_.at(uiListIndex_)->GetLongPress())) {
			XMFLOAT3 size = interactLongImage_->GetTextureSize();
			RECT rect;
			rect.left = 0;
			rect.top = 0;
			rect.right = (long)size.x;
			rect.bottom = (long)size.y;

			Transform tra = t;
			tra.position_.x += 0.5f;
			tra.position_.y += 0.5f;
			interactLongImage_->Draw(tra, rect, 1.0f);
		}

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
	//�G���[����
	if (uiListIndex_ <= -1 || uiListIndex_ > ((int)uiList_.size() - 1)) return false;

	if (uiList_.at(uiListIndex_)->GetParent() == parent) return true;
	return false;
}

void Interaction::SetParcent(float f)
{
	parcent_ = f;

}