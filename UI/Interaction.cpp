#include "Interaction.h"
#include "InteractionUI.h"
#include "../GameManager.h"
#include "../Player/Player.h"
#include "../Engine/Transform.h"
#include "../Engine/Global.h"
#include "../Engine/Sprite.h"
#include "../Engine/Camera.h"
#include "../Weapon/WeaponObjectManager.h"
#include <vector>

namespace Interaction {
	bool isDraw_;
	int uiListIndex_;
	Sprite* interactImage_;
	std::vector<InteractionUI*> uiList_;

	void Initialize() {
		interactImage_ = new Sprite();
		interactImage_->Load("Image/KeyImage/E.jpg");
		isDraw_ = false;
		uiListIndex_ = -1;
	}

	void Draw() {
		float minDistance = 99999;
		XMFLOAT3 pPos = GameManager::GetPlayer()->GetPosition();

		//WeaponObject‚ÌUiValid‚ðŒvŽZ‚·‚é
		if(rand() % 5 == 0) GameManager::GetWeaponObjectManager()->InteractUIIsInPlayerRange();

		for (int it = 0; it < (int)uiList_.size(); it++) {
			if (!uiList_[it]->GetValid()) continue;

			XMFLOAT3 pos = uiList_[it]->GetParent()->GetPosition();
			pos = { pos.x - pPos.x, 0.0f, pos.z - pPos.z };
			float range = sqrtf(pos.x * pos.x + pos.z * pos.z);
			if (range <= minDistance) {
				minDistance = range;
				isDraw_ = true;
				uiListIndex_ = it;
			}
		}

		if (isDraw_) {
			isDraw_ = false;
			XMFLOAT3 minPos = uiList_.at(uiListIndex_)->GetParent()->GetPosition();
			XMFLOAT3 of = uiList_.at(uiListIndex_)->GetOffset();
			minPos = { minPos.x + of.x, minPos.y + of.y, minPos.z + of.z };

			XMVECTOR v2 = XMVector3TransformCoord(XMLoadFloat3(&minPos), Camera::GetViewMatrix());
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

	void SceneTransitionInitialize()
	{
		ResetInteractData();

	}

	void ResetInteractData()
	{
		uiList_.clear();
	}

	void AddInteractData(InteractionUI* data)
	{
		uiList_.push_back(data);
	}

	void RemoveInteractData(InteractionUI* data)
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

	bool IsMinDistance(GameObject* parent)
	{
		if (uiListIndex_ <= -1 || uiListIndex_ > ((int)uiList_.size() - 1)) return false;

		if (uiList_.at(uiListIndex_)->GetParent() == parent) return true;
		return false;
	}

}