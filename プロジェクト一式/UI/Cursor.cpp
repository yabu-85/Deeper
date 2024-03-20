#include "Cursor.h"
#include "../Engine/Image.h"
#include "../Engine/Direct3D.h"
#include "../Engine/Input.h"
#include "../Engine/Global.h"
#include "../Engine/Transform.h"

namespace {
	XMFLOAT2 BUTTON_SIZE = { 0.5f, 0.5f };

}

Cursor::Cursor() : hPict_(-1), scale_(0.f, 0.f, 0.f)
{
	hPict_ = Image::Load("Image/Cursor.png");
	assert(hPict_ >= 0);

	scale_ = Float3Multiply(Image::GetTextureSize(hPict_), 0.5f);
	scale_ = { scale_.x / Direct3D::screenWidth_, scale_.y / Direct3D::screenHeight_, 1.0f };
}

Cursor::~Cursor()
{
}

void Cursor::Draw()
{
	Transform t;
	XMFLOAT3 cPos = Input::GetMousePosition();
	cPos = { cPos.x / Direct3D::screenWidth_, cPos.y / Direct3D::screenHeight_, 1.0f };
	t.position_ = { (cPos.x + scale_.x) * 2.0f - 1.0f, (cPos.y + scale_.y) * -2.0f + 1.0f, 1.0f };
	Image::SetTransform(hPict_, t);
	Image::Draw(hPict_, 0);

}
