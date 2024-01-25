#include "ActionImage.h"
#include <vector>
#include <utility>
#include "Engine/Text.h"
#include "GameManager.h"
#include "WeaponObjectManager.h"
#include "Engine/Image.h"
#include "Engine/Direct3D.h"

using std::string;

namespace {
	std::vector<std::vector<std::pair<string, int>>> explanations(MAX);

	enum KEY_NUMBER {
		M_LEFT,
		M_RIGHT,
		K_SPACE,
		K_E,
		K_Q,
		MAX,
	};

	const string keyName[10] = {
		"LEFT MOUSE",
		"RIGHT MOUSE",
		"SPACE KEY",
		"E KEY",
		"Q KEY",
	};

	Text* pText = nullptr;
	int drawPos[2] = {0, 0};

}

ActionImage::ActionImage(GameObject* parent)
	: GameObject(parent, "ActionImage"), isDraw_(true), isDrawAction_(true), type_(WAIT)
{
	for (int i = 0; i < 10; i++) hPict_[i] = -1;
}

ActionImage::~ActionImage()
{
}

void ActionImage::Initialize()
{
	SetName();
	pText = new Text();
	pText->Initialize();

	drawPos[0] = (int)((float)Direct3D::screenWidth_ / 2.0f * 1.2f);
	drawPos[1] = (int)((float)Direct3D::screenHeight_ / 2.0f * 0.3f);

	for (int i = 0; i < 10; i++) {
		hPict_[i] = Image::Load("Image/number/" + std::to_string(i) + ".png");
		assert(hPict_[i] >= 0);
	}

}

void ActionImage::Update()
{
}

void ActionImage::Draw()
{
	if (isDraw_) {

		//�e�[�u���̕\��
		int posY = 0;
		for (auto t : explanations[type_]) {
			string text = t.first + " : ";
			pText->Draw(drawPos[0], drawPos[1] + posY, text.c_str());

			//�s�N�Z���������n�_�E�オ�I�_
			float x = (float)Direct3D::screenWidth_ / 2.0f * 1.54f;
			float y = ((float)drawPos[1] + (float)posY);
			x = 2.0f * x / (float)Direct3D::screenWidth_ - 1.0f;
			y = 2.0f * y / (float)Direct3D::screenHeight_ - 1.0f;

			Transform tr;
			tr.position_ = XMFLOAT3(x, (y * -1.0f), 1.0f);

			Image::SetAlpha(hPict_[t.second], 255);
			Image::SetTransform(hPict_[t.second], tr);
			Image::Draw(hPict_[t.second]);
			
			posY += 40;

		}

		//�A�N�V�����{�^���̕\��
		//���낢�댩�ăt���O���
		bool hitFlag = GameManager::GetWeaponObjectManager()->IsInPlayerRange() || false;
		
		if (isDrawAction_ && hitFlag) {
			pText->Draw(drawPos[0], drawPos[1] + posY, "      EKey");
		}
	}

}

void ActionImage::Release()
{
}

void ActionImage::SetName()
{
	//�܂�������
	for (int i = 0; i < ACTION_UI_TABLE::MAX; i++) explanations[i].clear();

	//Wait
	explanations[WAIT].push_back({ "    ATTACK", M_LEFT });
	explanations[WAIT].push_back({ "ATTACK SUB", M_RIGHT });
	explanations[WAIT].push_back({ "    RORING", K_SPACE });

	//Walk
	explanations[WALK] = explanations[WAIT];

	//WEAPON_CHANGE
	explanations[WEAPON_CHANGE].push_back({ "ACTION", K_E });

	//AVO
	//explanations[AVO].push_back({ "", keyName[0] });

	//ATK
	explanations[ATK].push_back({"eee", M_LEFT });
	explanations[ATK].push_back({"fff", M_LEFT });
	explanations[ATK].push_back({"ggg", M_LEFT });

	//ATK_SUB
	explanations[ATK_SUB].push_back({ "hhh", M_LEFT });

}