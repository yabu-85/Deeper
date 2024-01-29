#include "Interaction.h"
#include "GameManager.h"
#include "WeaponObjectManager.h"
#include "WeaponObject.h"

#include "Engine/Transform.h"
#include "Engine/Global.h"
#include "Engine/Image.h"
#include "Engine/Camera.h"

namespace Interaction {
	float alpha_;
	bool  isDraw_;
	int hPict_ = -1;

	void Initialize() {
		hPict_ = Image::Load("Image/KeyImage/E.jpg");
		assert(hPict_ >= 0);

	}

	void Update() {

	}

	void Draw() {
		WeaponObject* w = GameManager::GetWeaponObjectManager()->GetNearestWeapon();
		if (!w) return;

		XMFLOAT3 pos = w->GetPosition();
		pos.y += 2.0f;
		XMVECTOR v2 = XMVector3TransformCoord(XMLoadFloat3(&pos), Camera::GetViewMatrix());
		v2 = XMVector3TransformCoord(v2, Camera::GetProjectionMatrix());
		float x = XMVectorGetX(v2);
		float y = XMVectorGetY(v2);

		Transform t;
		t.position_ = { x, y, 1.0f };
		t.scale_ = { 2.0f, 2.0f, 2.0f };
		Image::SetTransform(hPict_, t);
		Image::Draw(hPict_, 0);
	}

}