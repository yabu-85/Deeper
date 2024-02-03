#include "Interaction.h"
#include "../GameManager.h"
#include "../Weapon/WeaponObjectManager.h"
#include "../Weapon/WeaponObject.h"

#include "../Engine/SceneManager.h"
#include "../Engine/Transform.h"
#include "../Engine/Global.h"
#include "../Engine/Sprite.h"
#include "../Engine/Camera.h"

namespace Interaction {
	float alpha_;
	bool  isDraw_;
	Sprite* interactImage_;

	void Initialize() {
		interactImage_ = new Sprite();
		interactImage_->Load("Image/KeyImage/E.jpg");

	}

	void Update() {

	}

	void Draw() {
		//タイトルシーンだったら表示しない&表示する対象がなければ表示しない
		if (GameManager::GetSceneManager()->GetSceneID() == SCENE_ID_TITLE) return;
		WeaponObject* w = GameManager::GetWeaponObjectManager()->GetNearestWeapon();
		if (!w) return;

		XMFLOAT3 pos = w->GetPosition();
		pos.y += 2.0f;
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
		t.scale_ = { 2.0f, 2.0f, 2.0f };
		t.Calclation();
		interactImage_->Draw(t, rect, 1.0f, 1);

	}

}