#include "Interaction.h"
#include "../GameManager.h"
#include "../Weapon/WeaponObjectManager.h"
#include "../Weapon/WeaponObject.h"
#include "../Player/Player.h"

#include "../Engine/SceneManager.h"
#include "../Engine/Transform.h"
#include "../Engine/Global.h"
#include "../Engine/Sprite.h"
#include "../Engine/Camera.h"

namespace Interaction {
	bool isWarp_;
	bool  isDraw_;
	float minDistance_;
	XMFLOAT3 minPosition_;
	Sprite* interactImage_;

	void Initialize() {
		interactImage_ = new Sprite();
		interactImage_->Load("Image/KeyImage/E.jpg");
		isDraw_ = false;
		minDistance_ = 10000;
		isWarp_ = true;
	}

	void Update() {

	}

	void Draw() {		
		//•Ší‚ÌÅ¬‹——£FC³‰ÓŠ
		float height = 1.8f;
		isWarp_ = true;
		WeaponObject* w = GameManager::GetWeaponObjectManager()->GetNearestWeapon();
		if (w) {
			XMFLOAT3 pPos = GameManager::GetPlayer()->GetPosition();
			XMFLOAT3 wPos = w->GetPosition();
			XMVECTOR v = XMLoadFloat3(&pPos) - XMLoadFloat3(&wPos);
			float l = XMVectorGetX(XMVector3Length(v));
			if (l < minDistance_) {
				minPosition_ = w->GetPosition();
				minDistance_ = l;
				isDraw_ = true;
				height = 2.0f;
				isWarp_ = false;
			}
		}

		if (isDraw_) {
			minPosition_.y += height;
			XMVECTOR v2 = XMVector3TransformCoord(XMLoadFloat3(&minPosition_), Camera::GetViewMatrix());
			v2 = XMVector3TransformCoord(v2, Camera::GetProjectionMatrix());
			float x = XMVectorGetX(v2);
			float y = XMVectorGetY(v2);
			isDraw_ = false;
			minDistance_ = 10000.0f;

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

	void SetInteract(XMFLOAT3 pos)
	{
		XMFLOAT3 p = GameManager::GetPlayer()->GetPosition();
		XMVECTOR v = XMLoadFloat3(&pos) - XMLoadFloat3(&p);
		float l = XMVectorGetX(XMVector3Length(v));
		if (l < minDistance_) {
			minPosition_ = pos;
			minDistance_ = l;
			isDraw_ = true;
		}
	}

	void SetInteract(XMFLOAT3 pos, float length)
	{
		if (length < minDistance_) {
			minPosition_ = pos;
			minDistance_ = length;
			isDraw_ = true;
		}
	}

	bool IsWarp()
	{
		return isWarp_;
	}

}