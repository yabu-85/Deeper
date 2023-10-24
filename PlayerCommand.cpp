#include "PlayerCommand.h"
#include "Engine/Input.h"

PlayerCommand::PlayerCommand()
	:atk_(false), avo_(false), target_(false), left_(false), right_(false), up_(false), down_(false)
{
}

void PlayerCommand::Update()
{
	atk_ = Input::IsMouseButtonDown(0);
	avo_ = Input::IsKeyDown(DIK_SPACE);
	target_ = Input::IsKeyDown(DIK_Q);

	left_ = Input::IsKey(DIK_A);
	right_ = Input::IsKey(DIK_D);
	up_ = Input::IsKey(DIK_W);
	down_ = Input::IsKey(DIK_S);
	walk_ = left_ || right_ || up_ || down_;
}
