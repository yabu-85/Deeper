#include "PlayerCommand.h"
#include "Engine/Input.h"

//�f�o�b�O�p�H
#include "Engine/Text.h"
#include "Engine/Direct3D.h"

namespace {
	Text* text = new Text();
	bool keyDraw = false;
}

PlayerCommand::PlayerCommand()
	: walk_(false)
{
	SetDefaultKeyConfig();
	
	text->Initialize();

}

void PlayerCommand::Update()
{
	for (const auto& pair : pushKeyCommand_)
	commandFlags[pair.second] = Input::IsKey(pair.first);
	
	for (const auto& pair : downKeyCommand_)
	commandFlags[pair.second] = Input::IsKeyDown(pair.first);
	
	for (const auto& pair : pushMouseCommand_)
	commandFlags[pair.second] = Input::IsMouseButton(pair.first);
	
	for (const auto& pair : downMouseCommand_)
	commandFlags[pair.second] = Input::IsMouseButtonDown(pair.first);
	
	//Scroll : Walk
	commandFlags[CENTER_UP] = Input::IsUpScroll();
	commandFlags[CENTER_DOWN] = Input::IsDownScroll();
	walk_ = commandFlags[LEFT] || commandFlags[RIGHT] || commandFlags[UP] || commandFlags[DOWN];

	keyDraw = false;

}

void PlayerCommand::SetDefaultKeyConfig()
{
	pushKeyCommand_.clear();
	pushKeyCommand_.push_back(std::make_pair(DIK_A, LEFT));
	pushKeyCommand_.push_back(std::make_pair(DIK_D, RIGHT));
	pushKeyCommand_.push_back(std::make_pair(DIK_W, UP));
	pushKeyCommand_.push_back(std::make_pair(DIK_S, DOWN));
	pushKeyCommand_.push_back(std::make_pair(DIK_E, PUSH_ACTION));

	downKeyCommand_.clear();
	downKeyCommand_.push_back(std::make_pair(DIK_SPACE, AVO));
	downKeyCommand_.push_back(std::make_pair(DIK_Q, TARGET));
	downKeyCommand_.push_back(std::make_pair(DIK_E, DOWN_ACTION));

	pushMouseCommand_.clear();

	downMouseCommand_.clear();
	downMouseCommand_.push_back(std::make_pair(0, ATK));
	downMouseCommand_.push_back(std::make_pair(1, SUB_ATK));
	downMouseCommand_.push_back(std::make_pair(2, CENTER));

	SetKeyName();

}

void PlayerCommand::ChangeCmd(COMMAND number, int conf, int type)
{
	//�V�����ǉ������v�f�Ɠ������̂�����ꍇ�́E�E�E�Ȃ񂩂��H
	
	commandFlags[number] = false;
	DeleteCmd(number);

	if (type == 0) {
		pushKeyCommand_.push_back(std::make_pair(conf, number));
		return;
	}
	if (type == 1) {
		downKeyCommand_.push_back(std::make_pair(conf, number));
		return;
	}
	if (type == 2) {
		pushMouseCommand_.push_back(std::make_pair(conf, number));
		return;
	}
	if (type == 3) {
		downMouseCommand_.push_back(std::make_pair(conf, number));
		return;
	}

}

void PlayerCommand::DeleteCmd(COMMAND number)
{
	for (int i = 0; i < pushKeyCommand_.size(); i++) {
		if (pushKeyCommand_[i].second == number) {
			pushKeyCommand_.erase(pushKeyCommand_.begin() + i);
			return;
		}
	}

	for (int i = 0; i < downKeyCommand_.size(); i++) {
		if (downKeyCommand_[i].second == number) {
			downKeyCommand_.erase(downKeyCommand_.begin() + i);
			return;
		}
	}

	for (int i = 0; i < pushMouseCommand_.size(); i++) {
		if (pushMouseCommand_[i].second == number) {
			pushMouseCommand_.erase(pushMouseCommand_.begin() + i);
			return;
		}
	}

	for (int i = 0; i < downMouseCommand_.size(); i++) {
		if (downMouseCommand_[i].second == number) {
			downMouseCommand_.erase(downMouseCommand_.begin() + i);
			return;
		}
	}
}

void PlayerCommand::SetKeyName()
{
	keyName_[PUSH_ACTION] = "E";
	keyName_[DOWN_ACTION] = "E";

}

void PlayerCommand::DrawActionUI()
{
	if(keyDraw)
	text->Draw(Direct3D::screenWidth_ / 2, (int)((double)Direct3D::screenHeight_ / 2.0 * 1.8), keyName_[PUSH_ACTION].c_str());

}

void PlayerCommand::SetDrawActionUI()
{
	keyDraw = true;
}
