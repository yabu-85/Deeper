#pragma once
#include <map>

class PlayerCommand
{
	bool atk_;
	bool subAtk_;
	bool center_;
	bool centerUp_;
	bool centerDown_;
	bool avo_;
	bool target_;

	bool left_;
	bool right_;
	bool up_;
	bool down_;
	bool walk_;

public:
	PlayerCommand();
	void Update();

	bool CmdAtk() { return atk_; }
	bool CmdSubAtk() { return subAtk_; }
	bool CmdAvo() { return avo_; }
	bool CmdTarget() { return target_; }

	bool CmdCenter() { return center_; }
	bool CmdCenterUp() { return centerUp_; }
	bool CmdCenterDown() { return centerDown_; }

	bool CmdLeft() { return left_; }
	bool CmdRight() { return right_; }
	bool CmdUp() { return up_; }
	bool CmdDown() { return down_; }
	bool CmdWalk() { return walk_; }

};

