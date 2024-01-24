#include "ActionImage.h"
#include <vector>
#include "Engine/Text.h"
#include "Engine/Input.h"

namespace {
	std::vector<std::vector<std::string>> explanations(MAX);
	Text* pText = nullptr;
	int drawPos[2] = {20, 500};
	int drawPosAction[2] = {20, 550};

}

ActionImage::ActionImage(GameObject* parent)
	: GameObject(parent, "ActionImage"), isDraw_(true), isDrawAction_(true), type_(WAIT)
{
}

ActionImage::~ActionImage()
{
}

void ActionImage::Initialize()
{
	SetName();
	pText = new Text();
	pText->Initialize();

}

void ActionImage::Update()
{
	if (Input::IsKeyDown(DIK_1)) type_ = (ACTION_UI_TABLE)(rand() % MAX);

}

void ActionImage::Draw()
{
	if (isDraw_) {

		//テーブルの表示
		int posX = 0;
		for (auto t : explanations[type_]) {
			pText->Draw(drawPos[0], drawPos[1] + posX , t.c_str());
			posX += 40;

			//Direct3D::screenWidth_ / 2, (int)((double)Direct3D::screenHeight_ / 2.0 * 1.8),
		}

		//アクションボタンの表示
		if (isDrawAction_) {
			pText->Draw(drawPosAction[0], drawPosAction[1] + posX, "EKey");
		}
	}

	pText->Draw(30, 450, (int)(type_));


}

void ActionImage::Release()
{
}

void ActionImage::SetName()
{
	//Wait
	explanations[WAIT].push_back("aaa");
	explanations[WAIT].push_back("bbb");

	//Walk
	explanations[WALK].push_back("ccc");

	//WEAPON_CHANGE
	//explanations[WEAPON_CHANGE].push_back("");

	//AVO
	explanations[AVO].push_back("ddd");

	//ATK
	explanations[ATK].push_back("eee");
	explanations[ATK].push_back("fff");
	explanations[ATK].push_back("ggg");

	//ATK_SUB
	explanations[ATK_SUB].push_back("hhh");

}