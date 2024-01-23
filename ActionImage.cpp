#include "ActionImage.h"

ActionImage::ActionImage(GameObject* parent)
{
}

ActionImage::~ActionImage()
{
}

void ActionImage::Initialize()
{
}

void ActionImage::Update()
{
}

void ActionImage::Draw()
{
}

void ActionImage::Release()
{
}

void ActionImage::SetKeyName()
{
	keyName_[0] = "LEFT_MOUSE";
	keyName_[1] = "RIGHT_MOUSE";
	keyName_[2] = "CENTER";
	keyName_[3] = "SCROOL";
	keyName_[4] = "SCROOL";
	keyName_[5] = "SPACE";
	keyName_[6] = "Q";
	keyName_[7] = "E";
	keyName_[8] = "E";

}

void ActionImage::DrawActionUI()
{
	//if (keyDraw)
	//	text->Draw(Direct3D::screenWidth_ / 2, (int)((double)Direct3D::screenHeight_ / 2.0 * 1.8), keyName_[PUSH_ACTION].c_str());

}

void ActionImage::SetDrawActionUI()
{
	//keyDraw = true;
}
