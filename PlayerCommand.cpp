#include "PlayerCommand.h"
#include "Engine/Input.h"
#include <string>

PlayerCommand::PlayerCommand()
	: walk_(false)
{
	downKeyCommand_.push_back(std::make_pair(DIK_SPACE, AVO));
	downKeyCommand_.push_back(std::make_pair(DIK_Q, TARGET));

	pushKeyCommand_.push_back(std::make_pair(DIK_A, LEFT));
	pushKeyCommand_.push_back(std::make_pair(DIK_D, RIGHT));
	pushKeyCommand_.push_back(std::make_pair(DIK_W, UP));
	pushKeyCommand_.push_back(std::make_pair(DIK_S, DOWN));

	downMouseCommand_.push_back(std::make_pair(0, ATK));
	downMouseCommand_.push_back(std::make_pair(1, SUB_ATK));
	downMouseCommand_.push_back(std::make_pair(2, CENTER));

	scrollCommand_.push_back(std::make_pair(1, CENTER_UP));
	scrollCommand_.push_back(std::make_pair(-1, CENTER_DOWN));
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

	return;
}

void PlayerCommand::ChangeCmd(COMMAND number, int type)
{
	// 特定の要素を削除
	pushKeyCommand_.erase(pushKeyCommand_.begin() + 1); // 2番目の要素を削除

	// 削除後の要素へのアクセス
	for (const auto& pair : pushKeyCommand_) {
		int intValue = pair.first;
	}

}