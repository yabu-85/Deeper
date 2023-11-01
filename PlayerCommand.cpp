#include "PlayerCommand.h"
#include "Engine/Input.h"
#include <string>

PlayerCommand::PlayerCommand()
	:atk_(false), subAtk_(false),center_(false), avo_(false), target_(false), left_(false), right_(false), up_(false), down_(false), walk_(false),
	centerUp_(false), centerDown_(false)
{
}

void PlayerCommand::Update()
{
	atk_ = Input::IsMouseButtonDown(0);
	subAtk_ = Input::IsMouseButtonDown(1);
	
	centerUp_ = false;
	centerDown_ = false;
	if (Input::GetScroll() > 0.0f) centerUp_ = true;
	if (Input::GetScroll() < 0.0f) centerDown_ = true;
	
	avo_ = Input::IsKeyDown(DIK_SPACE);
	target_ = Input::IsKeyDown(DIK_Q);

	left_ = Input::IsKey(DIK_A);
	right_ = Input::IsKey(DIK_D);
	up_ = Input::IsKey(DIK_W);
	down_ = Input::IsKey(DIK_S);
	walk_ = left_ || right_ || up_ || down_;
}
